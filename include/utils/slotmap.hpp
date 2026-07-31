#include <vector>
#include <cstdint>
#include <optional>
#include <list>
#include <assert.h>
#include <iostream>

struct Slot {
	uint32_t index = 0;
	uint32_t generation = 0;

	Slot(){};
	Slot(uint32_t ind, uint32_t gen) : index(ind), generation(gen){};
};

template <typename T> 
class SlotMap {
	public:
		std::vector<Slot> indices;
		std::vector<T> values;
		std::list<Slot*> free_list;
		std::vector<uint32_t> index_ref; 
		uint32_t capacity;
		
		SlotMap(int reserve_size): capacity(reserve_size){
			this->indices = std::vector<Slot>(reserve_size); 
			this->values.reserve(reserve_size);
			this->index_ref.reserve(reserve_size);

			for(auto iterator = indices.begin(); iterator != indices.end(); ++iterator){
				this->free_list.push_back(std::to_address(iterator));
			}
		};
	
		//this is for quick access only insertion should be done with insertion function
		std::optional<T&> operator[](const Slot &key){
			assert(key.index < capacity && "Desired index Greater than size!");

			const Slot index_slot = this->indices[key.index];
			T &desired_value = this->values[index_slot.index];

			if(index_slot.generation != key.generation){ 
				return std::nullopt;
			} else {
//				std::cout << "KEY INDEX: " << key.index << " DATA INDEX: " << index_slot.index << std::endl;
				return desired_value;
			}
		};

		//ok you need to override the deleted object with the last object in the values array
		//destroy the last element
		//use the index from the initial indices to delete from index
		void erase(const Slot &key){
			assert(key.index < capacity && "Desired index Greater than Capacity!");

			Slot *deletion_slot = &this->indices[key.index];
			T& deletion_values = this->values[deletion_slot->index];
			uint32_t &del_slot_ref = this->index_ref[deletion_slot->index];

			if(key.generation != deletion_slot->generation) return;

			Slot &last_element = this->indices[this-> capacity - 1];
			T elem_values = this->values[last_element.index];
			uint32_t elem_slot_ref = this->index_ref[last_element.index];

			//overwrite the values and index ref to be deleted with the last elements values 
			deletion_values = elem_values;
			del_slot_ref = elem_slot_ref;
			last_element.index = deletion_slot->index;

			//push the deletion slots pointer onto the free list
			deletion_slot->generation += 1;
			this->free_list.push_back(deletion_slot);

			//pop the last element from the values array and index element array
			this->values.pop_back();
			this->index_ref.pop_back();
		}

		Slot insert(T value){
			//WORKHERE
			//check to see if freelist has elements
			if(this->free_list.empty()){
				assert(this->values.size() == this->values.capacity() && "Vectors Allocating allocating when more space is available!");

				//resize all my vectors and set capacity
				this->indices.resize(this->capacity * 2); //indices should always be equal to capacity
									  
				this->values.reserve(this->capacity * 2);
				this->index_ref.reserve(this->capacity * 2);

				//put more elements in the free list
				for(auto iterator = this->indices.begin() + capacity; iterator != this->indices.end(); ++iterator){
					this->free_list.push_back(std::to_address(iterator));
				}
				this->capacity *= 2;

				assert(this->indices.capacity() == this->values.capacity() and this->index_ref.capacity() == this->indices.capacity() && "vectors do not have the same capacity!");
			}


			Slot* free_ptr = this->free_list.front();
			uint32_t slot_index = std::distance(this->indices.data(), free_ptr);
			this->free_list.pop_front();

//			std::cout << "NEW SLOT INDEX: " << slot_index << " DATA INDEX: " << this->values.size() <<" VALUE: " << value << std::endl;
			free_ptr->index = this->values.size();

			this->values.push_back(value);
			this->index_ref.push_back(slot_index);

			assert(this->values.size() == this->index_ref.size() && "value and index_ref containers do not have the same length!");

			Slot key(slot_index, free_ptr->generation);
			return key;
		}
};
