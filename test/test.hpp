#include <gtest/gtest.h>
#include <utils/slotmap.hpp>

//includes a slotmap with slots that have references to data
//WARN: If insertion test fails then all test using this fixture are invalid
class SlotMapTestFixture : public testing::Test {
	protected:
	SlotMap<int> test_map;
	std::vector<Slot> slots;

	void SetUp() override{
		test_map = SlotMap<int>(10);

		for(int i = 0; i < 10; ++i){
			//reach the capacity
			this->slots.push_back(test_map.insert(i));
		}
	}
};
