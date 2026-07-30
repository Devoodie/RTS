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

template <typename T>
const std::optional<T&> SlotMap<T>::operator[](const Slot &slot){
	assert(slot.index < size && "Desired index Greater than size!");
	T &desired_data = this->data[slot.index];

	uint32_t metadeta_index = this->index_ref[slot.index];
	Slot meta_data = this->indices[metadeta_index];

	if(meta_data.generation != slot.generation or meta_data.index != slot.index){ 
		return nullptr;
	} else {
		return desired_data;
	}
}

template <typename T>
Slot SlotMap<T>::insert(const Slot slot){
	//WORKHERE
}
