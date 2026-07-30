#include <vector>
#include <cstdint>
#include <optional>

struct Slot {
	uint32_t index;
	uint32_t generation;
};

template <typename T> 
class SlotMap {
	public:
		std::vector<Slot> indices;
		std::vector<T> data;
		std::vector<uint32_t> erase; //names not very expressive but this maps directly onto the slotmap definition from the video i watched
		uint32_t size;
		
		SlotMap(int reserve_size);

		std::optional<T&> operator[](const Slot &slot);
};
