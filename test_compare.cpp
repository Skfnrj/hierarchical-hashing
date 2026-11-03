#include <iostream>
#include <cassert>
#include "sha256.h"
#include "merkle_tree.h"

int main(){
    SHA256 sha;
    MerkleTree first(&sha);
    MerkleTree second(&sha);
    std::vector<int> differences;

    //identical inputs, including an empty item
    first.build_tree({"abc", "", "xyz"});
    second.build_tree({"abc", "", "xyz"});

    assert(first.compare_tree(second, differences));
    assert(differences.empty());

    //change the final leaf in an odd-sized tree
    second.build_tree({"abc", "", "changed"});

    assert(first.compare_tree(second, differences));
    assert(differences.size() == 1);
    assert(differences[0] == 2);

    //change the first two inputs
    second.build_tree({"changed", "hello", "xyz"});

    assert(first.compare_tree(second, differences));
    assert(differences.size() == 2);
    assert(differences[0] == 0);
    assert(differences[1] == 1);

    //different counts must be rejected, even if roots match
    first.build_tree({"abc", "", "xyz"});
    second.build_tree({"abc", "", "xyz", "xyz"});

    assert(first.get_root_hash() == second.get_root_hash());
    assert(!first.compare_tree(second, differences));
    assert(differences.empty());

    //two empty trees
    first.build_tree({});
    second.build_tree({});

    assert(first.compare_tree(second, differences));
    assert(differences.empty());

    //empty versus nonempty
    second.build_tree({"abc"});

    assert(!first.compare_tree(second, differences));

    //a separate hashing object is rejected by the current rule
    SHA256 another_sha;
    MerkleTree third(&another_sha);
    first.build_tree({"abc"});
    third.build_tree({"abc"});

    assert(!first.compare_tree(third, differences));

    std::cout << "All comparison tests passed\n";
    return 0;
}