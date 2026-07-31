#include <gtest/gtest.h>
#include <vector>
#include <utils/slotmap.hpp>

TEST(SlotMap, Insertion){
	SlotMap<int> test_map(10);
	std::vector<Slot> slots;

	ASSERT_EQ(test_map.index_ref.capacity(), test_map.indices.capacity()) << "All vectors do not have equal capacity";
	ASSERT_EQ(test_map.values.capacity(), test_map.indices.capacity()) << "All vectors do not have equal capacity";
	ASSERT_EQ(test_map.capacity, test_map.indices.capacity()) << "All vectors do not have capacity equal to capacity property";

	for(int i = 0; i < 10; ++i){
		//reach the capacity
		slots.push_back(test_map.Insert(i));
	}

	//verify data
	for(int i = 0; i < 10; ++i){
		//reach the capacity
		std::optional<int&> value = test_map[slots[i]];
		if(value){
			ASSERT_EQ(*value, i) << "Slot map data is not equal to inserted data. EXPECTED VALUE: " << i << ", RETURNED VALUE: " << *value;
		} else {
			ASSERT_NE(value, std::nullopt) << "Slot map data is equal to null";
		}
	}
	ASSERT_EQ(test_map.free_list.empty(), true) << "Free list is not empty.";

	int current_cap = test_map.capacity;

	slots.push_back(test_map.Insert(10));

	ASSERT_EQ(test_map.capacity, current_cap * 2) << "Capcity didnt grow to 2 times the previous capacity";

	ASSERT_EQ(test_map.index_ref.capacity(), test_map.indices.capacity()) << "All vectors do not have equal capacity";
	ASSERT_EQ(test_map.values.capacity(), test_map.indices.capacity()) << "All vectors do not have equal capacity";
	ASSERT_EQ(test_map.capacity, test_map.indices.capacity()) << "All vectors do not have capacity equal to capacity property";

	ASSERT_EQ(test_map.free_list.size(), current_cap - 1) << "Free list did not add new elements after allocation";
}

//includes a slotmap with slots that have references to data
//WARN: If insertion test fails then all test using this fixture are invalid
class SlotMapTestFixture : public testing::Test {
	protected:
	SlotMap<int> test_map;
	std::vector<Slot> slots;

	SlotMapTestFixture() : test_map(10) {
		for(int i = 0; i < 10; ++i){
			//reach the capacity
			this->slots.push_back(test_map.Insert(i));
		}
	} 
};

TEST_F(SlotMapTestFixture, Erase){
	//take slot from the middle index 
	Slot key = slots[5];
	Slot *deletion_slot = &test_map.indices[5];
	int &deletion_data = *test_map[key]; // we know its valid
	uint32_t generation = deletion_slot->generation;

	int last_data_elem = test_map.values[9];
	uint32_t last_slot_ref = test_map.index_ref[9];
	Slot &last_elem_slot = test_map.indices[last_slot_ref];

	test_map.erase(key);

	ASSERT_EQ(deletion_slot, this->test_map.free_list.back()) << "Erased element was not appended to the back of the free list";
	ASSERT_EQ(deletion_data, last_data_elem) << "Overwritten 'erased data' is not equal to the data that was at the end of the data vector";
	ASSERT_EQ(deletion_slot->index, last_elem_slot.index) << "The (formerly) Last elements slot does not reference its new swapped position with overwritten element";
	ASSERT_EQ(deletion_slot->generation, generation + 1) << "The erased slots generation was not incremented after it was erased/overwritten";
	ASSERT_EQ(test_map[key], std::nullopt) << "The SlotMap does not return nullopt when indexed by the erased key";

	Slot new_key = test_map.Insert(5);

	ASSERT_EQ(test_map.capacity, 10) << "SlotMap did not use recently freed element for insertion";
}
