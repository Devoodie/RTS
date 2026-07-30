#include <vector>
#include <cstdint>
#include <optional>
#include <list>

struct Slot {
	uint32_t index;
	uint32_t generation;
};

template <typename T> 
class SlotMap {
	public:
		std::vector<Slot> indices;
		std::vector<T> data;
		std::list<Slot*> free_list;
		std::vector<uint32_t> index_ref; 
		uint32_t capacity;
		
		SlotMap(int reserve_size);

		//this is for quick access only insertion should be done with insertion function
		const std::optional<T&> operator[](const Slot &key);

		void erase(const Slot &key);
		Slot insert(const Slot slot);
};
