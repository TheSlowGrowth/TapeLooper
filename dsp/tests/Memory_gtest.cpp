/**	
 * Copyright (C) Johannes Elliesen, 2021
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <array>
#include <memory>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <util/Memory.h>

using ::testing::_;

TEST(StackMemoryProvider, a_write)
{
    StackMemoryProvider<4> mem;
    const uint32_t src = 123456789u;
    mem.write(&src, sizeof(uint32_t));
    EXPECT_EQ(*(uint32_t*) (mem.arr.data()), 123456789u);
    EXPECT_EQ(mem.ptr, mem.arr.data() + sizeof(uint32_t));
}

TEST(StackMemoryProvider, b_read)
{
    StackMemoryProvider<4> mem;
    *(uint32_t*) (mem.arr.data()) = 123456789u;
    uint32_t dest;
    mem.read(&dest, sizeof(uint32_t));
    EXPECT_EQ(dest, 123456789u);
}

/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////

class MemoryProviderMock
{
public:
    MOCK_METHOD(size_t, getAvailableSize, (), (const));
    MOCK_METHOD(void, write, (const void* src, size_t sizeToWrite), ());
    MOCK_METHOD(void, read, (void* dest, size_t sizeToRead), ());
};

TEST(WritableMemory, a_dontAlterContentsOnConstruction)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    // call getAvailableSize() just once
    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(2ul));
    // No other calls expected.
    EXPECT_CALL(memProvider, read(_, _)).Times(0);
    EXPECT_CALL(memProvider, write(_, _)).Times(0);

    WritableMemory mem(memProvider);
}

TEST(WritableMemory, b_writeSingleItem)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    const uint8_t value1 = 12;
    const uint16_t value2 = 345;

    ::testing::Sequence expectCallToHappenInSequence;

    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(3ul));
    EXPECT_CALL(memProvider, read(_, _)).Times(0);
    EXPECT_CALL(memProvider, write((const void*) &value1, sizeof(value1))).Times(1);
    EXPECT_CALL(memProvider, write((const void*) &value2, sizeof(value2))).Times(1);

    WritableMemory mem(memProvider);
    EXPECT_EQ(mem.getRemainingSize(), 3ul);
    EXPECT_TRUE(mem.writeItem(value1));
    EXPECT_EQ(mem.getRemainingSize(), 2ul);
    EXPECT_TRUE(mem.writeItem(value2));
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
    EXPECT_FALSE(mem.writeItem(value1)); // out of memory now!
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
}

TEST(WritableMemory, c_writeMultipleItems)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    const uint8_t value1 = 12;
    const uint16_t value2 = 345;

    ::testing::Sequence expectCallToHappenInSequence;

    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(3ul));
    EXPECT_CALL(memProvider, read(_, _)).Times(0);
    EXPECT_CALL(memProvider, write((const void*) &value1, sizeof(value1))).Times(1);
    EXPECT_CALL(memProvider, write((const void*) &value2, sizeof(value2))).Times(1);

    WritableMemory mem(memProvider);
    EXPECT_EQ(mem.getRemainingSize(), 3ul);
    EXPECT_TRUE(mem.writeItems(value1, value2));
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
    EXPECT_FALSE(mem.writeItems(value1, value2)); // out of memory now!
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
}

TEST(WritableMemory, d_writeRaw)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    const uint8_t value1 = 12;
    const uint16_t value2 = 234;

    ::testing::Sequence expectCallToHappenInSequence;

    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(3ul));
    EXPECT_CALL(memProvider, read(_, _)).Times(0);
    EXPECT_CALL(memProvider, write((const void*) &value1, sizeof(value1))).Times(1);
    EXPECT_CALL(memProvider, write((const void*) &value2, sizeof(value2))).Times(1);

    WritableMemory mem(memProvider);
    EXPECT_EQ(mem.getRemainingSize(), 3ul);
    EXPECT_TRUE(mem.writeRaw((void*) &value1, sizeof(value1)));
    EXPECT_EQ(mem.getRemainingSize(), 2ul);
    EXPECT_TRUE(mem.writeRaw((void*) &value2, sizeof(value2)));
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
    EXPECT_FALSE(mem.writeRaw((void*) &value1, sizeof(value1))); // out of memory now!
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
}

/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////

TEST(ReadableMemory, a_dontReadOnConstruction)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    // call getAvailableSize() just once
    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(2ul));
    // No other calls expected.
    EXPECT_CALL(memProvider, read(_, _)).Times(0);
    EXPECT_CALL(memProvider, write(_, _)).Times(0);

    ReadableMemory mem(memProvider);
}

TEST(ReadableMemory, b_readSingleitem)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    uint8_t value1 = 0;
    uint16_t value2 = 0;

    ::testing::Sequence expectCallToHappenInSequence;

    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(3ul));
    EXPECT_CALL(memProvider, read((void*) &value1, sizeof(value1)))
        .WillOnce(testing::Invoke([](void* dest, size_t) {
            *((uint8_t*) dest) = 12u;
        }));
    EXPECT_CALL(memProvider, read((void*) &value2, sizeof(value2)))
        .WillOnce(testing::Invoke([](void* dest, size_t) {
            *((uint16_t*) dest) = 234u;
        }));
    EXPECT_CALL(memProvider, write(_, _)).Times(0);

    ReadableMemory mem(memProvider);
    EXPECT_EQ(mem.getRemainingSize(), 3ul);
    EXPECT_TRUE(mem.readItem(value1));
    EXPECT_EQ(value1, 12);
    EXPECT_EQ(mem.getRemainingSize(), 2ul);
    EXPECT_TRUE(mem.readItem(value2));
    EXPECT_EQ(value2, 234);
    EXPECT_EQ(mem.getRemainingSize(), 0ul);

    uint8_t value3 = 123;
    EXPECT_FALSE(mem.readItem(value3)); // out of memory now!
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
}

TEST(ReadableMemory, c_readMultipleItems)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    uint8_t value1 = 0;
    uint16_t value2 = 0;

    ::testing::Sequence expectCallToHappenInSequence;

    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(3ul));
    EXPECT_CALL(memProvider, read((void*) &value1, sizeof(value1)))
        .WillOnce(testing::Invoke([](void* dest, size_t) {
            *((uint8_t*) dest) = 12u;
        }));
    EXPECT_CALL(memProvider, read((void*) &value2, sizeof(value2)))
        .WillOnce(testing::Invoke([](void* dest, size_t) {
            *((uint16_t*) dest) = 234u;
        }));
    EXPECT_CALL(memProvider, write(_, _)).Times(0);

    ReadableMemory mem(memProvider);
    EXPECT_EQ(mem.getRemainingSize(), 3ul);
    EXPECT_TRUE(mem.readItems(value1, value2));
    EXPECT_EQ(value1, 12);
    EXPECT_EQ(value2, 234);
    EXPECT_EQ(mem.getRemainingSize(), 0ul);

    uint8_t value3 = 123;
    EXPECT_FALSE(mem.readItem(value3)); // out of memory now!
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
}

TEST(ReadableMemory, d_readRaw)
{
    ::testing::StrictMock<MemoryProviderMock> memProvider;

    uint8_t value1 = 0;
    uint16_t value2 = 0;

    ::testing::Sequence expectCallToHappenInSequence;

    EXPECT_CALL(memProvider, getAvailableSize())
        .WillOnce(::testing::Return(3ul));
    EXPECT_CALL(memProvider, read((void*) &value1, sizeof(value1)))
        .WillOnce(testing::Invoke([](void* dest, size_t) {
            *((uint8_t*) dest) = 12u;
        }));
    EXPECT_CALL(memProvider, read((void*) &value2, sizeof(value2)))
        .WillOnce(testing::Invoke([](void* dest, size_t) {
            *((uint16_t*) dest) = 234u;
        }));
    EXPECT_CALL(memProvider, write(_, _)).Times(0);

    ReadableMemory mem(memProvider);
    EXPECT_EQ(mem.getRemainingSize(), 3ul);
    EXPECT_TRUE(mem.readRaw((void*) &value1, sizeof(value1)));
    EXPECT_EQ(value1, 12);
    EXPECT_EQ(mem.getRemainingSize(), 2ul);
    EXPECT_TRUE(mem.readRaw((void*) &value2, sizeof(value2)));
    EXPECT_EQ(value2, 234);
    EXPECT_EQ(mem.getRemainingSize(), 0ul);

    uint8_t value3 = 123;
    EXPECT_FALSE(mem.readRaw((void*) &value3, sizeof(value3))); // out of memory now!
    EXPECT_EQ(mem.getRemainingSize(), 0ul);
}
