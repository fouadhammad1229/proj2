#include "List.h"
#include <iostream>
#include <string>
#include <limits>

class MyMemoryBlock {
private:
    int index;
    int length;

public:
    MyMemoryBlock(int ind = 0, int len = 0) : index(ind), length(len) {}

    int get_length() const { return length; }
    int get_index() const { return index; }

    int allocate_mem(int len) {
        if (len > length) return -1;
        length -= len;
        return len;
    }

    void free_mem(int len) {
        length += len;
    }
};

class MyMemory {
private:
    cop4530::List<MyMemoryBlock> mlist;
    int max_index;
    int block_size;

public:
    MyMemory(int num = 0, int len = 0) {
        resize(num, len);
    }

    void resize(int num = 100, int len = 100) {
        mlist.clear();
        max_index = num - 1;
        block_size = len;
        for (int i = 0; i <= max_index; ++i) {
            mlist.push_back(MyMemoryBlock(i, block_size));
        }
    }

    int req_mem(int len) {
        for (auto it = mlist.begin(); it != mlist.end(); ++it) {
            int allocated = it->allocate_mem(len);
            if (allocated != -1) {
                if (it->get_length() == 0) {
                    mlist.erase(it);
                }
                return it->get_index();
            }
        }
        return -1;
    }

    bool release_mem(int ind, int len) {
        if (ind < 0 || ind > max_index || len < 0 || len > block_size) {
            return false;
        }

        for (auto it = mlist.begin(); it != mlist.end(); ++it) {
            if (it->get_index() == ind) {
                it->free_mem(len);
                return true;
            } else if (it->get_index() > ind) {
                mlist.insert(it, MyMemoryBlock(ind, len));
                return true;
            }
        }

        mlist.push_back(MyMemoryBlock(ind, len));
        return true;
    }

    void merge_mem() {
        auto it = mlist.begin();
        while (it != mlist.end()) {
            auto current = it;
            ++it;
            if (it == mlist.end()) break;

            if (current->get_length() + it->get_length() <= block_size) {
                current->free_mem(it->get_length());
                it = mlist.erase(it);
                it = current;
            }
        }
    }

    void dump() const {
        std::cout << "Number of blocks: " << mlist.size() << std::endl;
        for (const auto& block : mlist) {
            std::cout << "Block(index = " << block.get_index() << "): " << block.get_length() << std::endl;
        }
    }

    ~MyMemory() = default;
};

void print_menu() {
    std::cout << std::string(20, '=') << std::endl;
    std::cout << "d: memory dump" << std::endl;
    std::cout << "r: request memory" << std::endl;
    std::cout << "l: release memory" << std::endl;
    std::cout << "m: merge memory" << std::endl;
    std::cout << "x: quit program" << std::endl;
    std::cout << "Your choice: ";    
}

int main() {
    int num_blocks, block_size;
    
    std::cout << "Memory configuration:" << std::endl;
    std::cout << "Number of memory blocks (integer): ";
    std::cin >> num_blocks;
    std::cout << "Memory block size (integer): ";
    std::cin >> block_size;
    
    MyMemory mem(num_blocks, block_size);
    char choice;

    do {
        print_menu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "choice: " << choice << std::endl;

        switch (choice) {
            case 'd':
                mem.dump();
                break;
            case 'r': {
                int len;
                std::cout << "Enter the amount of memory to request: ";
                std::cin >> len;
                int index = mem.req_mem(len);
                if (index != -1) {
                    std::cout << "Memory allocated at index: " << index << std::endl;
                } else {
                    std::cout << "Failed to allocate memory." << std::endl;
                }
                break;
            }
            case 'l': {
                int index, len;
                std::cout << "Enter the index to release memory from: ";
                std::cin >> index;
                std::cout << "Enter the amount of memory to release: ";
                std::cin >> len;
                if (mem.release_mem(index, len)) {
                    std::cout << "Memory released successfully." << std::endl;
                } else {
                    std::cout << "Failed to release memory." << std::endl;
                }
                break;
            }
            case 'm':
                mem.merge_mem();
                std::cout << "Memory blocks merged." << std::endl;
                break;
            case 'x':
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }

        if (choice != 'x') {
            std::cout << std::endl;
        }
    } while (choice != 'x');

    return 0;
}
