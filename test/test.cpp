#include <test.hpp>
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
		slots.push_back(test_map.insert(i));
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

	slots.push_back(test_map.insert(10));

	ASSERT_EQ(test_map.capacity, current_cap * 2) << "Capcity didnt grow to 2 times the previous capacity";

	ASSERT_EQ(test_map.index_ref.capacity(), test_map.indices.capacity()) << "All vectors do not have equal capacity";
	ASSERT_EQ(test_map.values.capacity(), test_map.indices.capacity()) << "All vectors do not have equal capacity";
	ASSERT_EQ(test_map.capacity, test_map.indices.capacity()) << "All vectors do not have capacity equal to capacity property";

	ASSERT_EQ(test_map.free_list.size(), current_cap - 1) << "Free list did not add new elements after allocation";
}

TEST_F(SlotMapTestFixture, Erase){
	//take slot from the middle index 
	Slot key = slots[5];
	Slot *deletion_slot = &test_map.indices[5];

	int last_data_elem = test_map.values[9];
}
