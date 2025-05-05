#include "ht.h"
#include "hash.h"
#include <iostream>
#include <string>
#include <set>
#include <cassert>

using namespace std;

// Helper function to verify that all items are in the hash table
bool verifyItems(
    HashTable<string, int, DoubleHashProber<string, std::hash<string>>, hash<string>, equal_to<string>>& ht,
    const set<pair<string, int>>& items)
{
    for (const auto& item : items) {
        auto found = ht.find(item.first);
        if (found == nullptr || *found != item) {
            return false;
        }
    }
    return true;
}

int main()
{
    // Create a hash table with the default alpha factor of 0.4
    HashTable<string, int, DoubleHashProber<string, std::hash<string>>, hash<string>, equal_to<string>> ht;

    // Use a set to track inserted items
    set<pair<string, int>> items;

    // Insert 5 items to reach the load factor threshold
    cout << "Inserting 5 items..." << endl;
    for (int i = 0; i < 5; i++) {
        pair<string, int> newItem(to_string(i), i);
        ht.insert(newItem);
        items.insert(newItem);
    }

    // Verify that the table size is still 11 (no resize yet)
    assert(ht.tableSize() == 11);
    cout << "Table size after 5 inserts: " << ht.tableSize() << " (expected: 11)" << endl;

    // Insert one more item to trigger a resize
    cout << "Inserting 6th item to trigger resize..." << endl;
    pair<string, int> newItem(to_string(5), 5);
    ht.insert(newItem);
    items.insert(newItem);

    // Verify that the table resized to the next capacity (23)
    assert(ht.tableSize() == 23);
    cout << "Table size after resize: " << ht.tableSize() << " (expected: 23)" << endl;

    // Verify that all items are still in the hash table
    assert(verifyItems(ht, items));
    cout << "All items verified after resize." << endl;

    cout << "All tests passed!" << endl;
    return 0;
}
