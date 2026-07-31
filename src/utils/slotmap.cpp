#include <cassert>
#include <iterator>
#include <utils/slotmap.hpp>
#include <memory>
#include <iterator>

template <typename T>
SlotMap<T>::SlotMap(int reserve_size) : capacity(reserve_size) {
	this->indices = std::vector<Slot>(reserve_size); 
	this->values = std::vector<T>(reserve_size);
	this->index_ref = std::vector<uint32_t>(reserve_size);

	for(auto iterator = indices.begin(); iterator != indices.end(); ++iterator){
		this->free_list.push_back(std::to_address(iterator));
	}
};

//use the key to get the slot that will index the values array
template <typename T>
const std::optional<T&> SlotMap<T>::operator[](const Slot &key){
	assert(key.index < capacity && "Desired index Greater than size!");

	const Slot index_slot = this->indices[key.index];
	T &desired_value = this->values[index_slot.index];

	if(index_slot.generation != key.generation){ 
		return nullptr;
	} else {
		return desired_value;
	}
}

//ok you need to override the deleted object with the last object in the values array
//destroy the last element
//use the index from the initial indices to delete from index
template <typename T>
void SlotMap<T>::erase(const Slot &key){
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

template <typename T>
Slot SlotMap<T>::insert(T value){
	//WORKHERE
	//check to see if freelist has elements
	if(this->free_list.empty()){
		assert(this->values.size() == this->values.capacity() && "Vectors Allocating allocating when more space is available!");

		//resize all my vectors and set capacity
		this->indices.resize(this->capacity * 2);
		this->values.resize(this->capacity * 2);
		this->index_ref.resize(this->capacity * 2);

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

	free_ptr->index = this->values.size();

	this->values.push_back(value);
	this->index_ref.push_back(slot_index);

	assert(this->values.size() == this->index_ref.size() && "value and index_ref containers do not have the same length!");

	Slot key(slot_index, free_ptr->generation);
	return key;
}
