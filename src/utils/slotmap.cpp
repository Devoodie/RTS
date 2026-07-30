#include <cassert>
#include <utils/slotmap.hpp>
#include <iostream>

template <typename T>
SlotMap<T>::SlotMap(int reserve_size) : size(reserve_size) {
	this->indices = std::vector<Slot>(reserve_size); 
	this->data = std::vector<T>(reserve_size);
	this->erase = std::vector<uint32_t>(reserve_size);
};

template <typename T>
std::optional<T&> SlotMap<T>::operator[](const Slot &slot){
	assert(slot.index < size && "Desired index Greater than size!");
	T &desired_data = this->data[slot.index];

	uint32_t metadeta_index = this->erase[slot.index];
	Slot meta_data = this->indices[metadeta_index];

	if(meta_data.generation != slot.generation or meta_data.index != slot.index){ 
		return nullptr;
	} else {
		return desired_data;
	}
}
