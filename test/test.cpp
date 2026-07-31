#include <cassert>
#include <gtest/gtest.h>
#include <utils/slotmap.hpp>
#include <vector>
TEST(SlotMap, Insertion){
	SlotMap<int> test_map(10);
	std::vector<Slot> slots(10);

	ASSERT_EQ(test_map.index_ref.capacity(), test_map.indices.capacity()) << "All vectors have equal capacity";
	ASSERT_EQ(test_map.values.capacity(), test_map.indices.capacity()) << "All vectors have equal capacity";
	ASSERT_EQ(test_map.capacity, test_map.indices.capacity()) << "All vectors have capacity equal to capacity property";

	for(int i = 0; i < 10; ++i){
		//reach the capacity
		slots.push_back(test_map.insert(i));
	}
	//verify data
	
	for(int i = 0; i < 10; ++i){
		//reach the capacity
		int &value = test_map[slots[i]];
		ASSERT_EQ(test_map[slot],)
	}

}
