#include <cassert>
#include <utils/slotmap.hpp>
#include <memory>


template <typename T>
SlotMap<T>::SlotMap(int reserve_size) : capacity(reserve_size) {
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
	assert(key.index < capacity && "Desired index Greater than size!");

	const Slot index_slot = this->indices[key.index];
	T &desired_data = this->data[index_slot.index];

	if(index_slot.generation != key.generation){ 
		return nullptr;
	} else {
		return desired_data;
	}
}
//TODO >> make sure generation matches before deletion

//ok you need to override the deleted object with the last object in the data array
//destroy the last element
//use the index from the initial indices to delete from index
template <typename T>
void SlotMap<T>::erase(const Slot &key){
	assert(key.index < capacity && "Desired index Greater than Capacity!");

	Slot *deletion_slot = &this->indices[key.index];
	T& deletion_data = this->data[deletion_slot->index];
	uint32_t &del_slot_ref = this->index_ref[deletion_slot->index];

	Slot &last_element = this->indices[this-> capacity - 1];
	T elem_data = this->data[last_element.index];
	uint32_t elem_slot_ref = this->index_ref[last_element.index];

	//overwrite the data and index ref to be deleted with the last elements data 
	deletion_data = elem_data;
	del_slot_ref = elem_slot_ref;
	last_element.index = deletion_slot->index;

	//push the deletion slots pointer onto the free list
	deletion_slot->generation += 1;
	this->free_list.push_back(deletion_slot);

	//pop the last element from the data array and index element array
	this->data.pop_back();
	this->index_ref.pop_back();
}

template <typename T>
Slot SlotMap<T>::insert(const Slot slot){
	//WORKHERE
}
