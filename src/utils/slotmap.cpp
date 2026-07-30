#include <cassert>
#include <utils/slotmap.hpp>
#include <memory>


template <typename T>
SlotMap<T>::SlotMap(int reserve_size) : size(reserve_size) {
	this->indices = std::vector<Slot>(reserve_size); 
	this->data = std::vector<T>(reserve_size);
	this->index_ref = std::vector<uint32_t>(reserve_size);

	for(auto iterator = indices.begin(); iterator != indices.end(); ++iterator){
		this->free_list.push_back(std::to_address(iterator));
	}
};

//use the key to get the slot that will index the data array
template <typename T>
const std::optional<T&> SlotMap<T>::operator[](const Slot &key){
	assert(key.index < size && "Desired index Greater than size!");

	const Slot index_slot = this->indices[key.index];
	T &desired_data = this->data[index_slot.index];

	if(index_slot.generation != key.generation){ 
		return nullptr;
	} else {
		return desired_data;
	}
}

//ok you need to override the deleted object with the last object in the data array
//destroy the last element
//use the index from the initial indices to delete from index
template <typename T>
void SlotMap<T>::erase(const Slot &key){
	assert(key.index < size && "Desired index Greater than size!");

	const Slot index_slot = this->indices[key.index];
	int index = this->data[index_slot];

	T &desired_data = this->data[index_slot.index];
}

template <typename T>
Slot SlotMap<T>::insert(const Slot slot){
	//WORKHERE
}
