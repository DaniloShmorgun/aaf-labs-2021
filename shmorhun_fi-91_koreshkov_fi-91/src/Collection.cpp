#include "Collection.h"
#include <stack>

void Collection::Insert(const Set& new_set) {
    if (!root) {
        root = new Node(nullptr, new_set);
    }
    else {
        root->Insert(new_set);
    }
}


void Collection::PrintSubtree(std::ostream& os, Node* subtree_root, std::string prefix, std::string children_prefix) const {
    if (subtree_root == nullptr) {
        return;
    }
    os << prefix << subtree_root->set;
    if (subtree_root->is_real) os << "*";
    if (subtree_root->subnodes[0] == nullptr && subtree_root->subnodes[1] == nullptr) os << "]";
    os << std::endl;

    PrintSubtree(os, subtree_root->subnodes[0], children_prefix + "L-- ", children_prefix + "|   ");
    PrintSubtree(os, subtree_root->subnodes[1], children_prefix + "L-- ", children_prefix + "    ");
}

void Collection::Print(std::ostream& os) const {
    if (!root) {
        os << "Empty Collection";
        return;
    }

    PrintSubtree(os, root, "", "");

}




void Node::ExpandTo(const Set& to_set) {
    set = set.Union(to_set);
}

/*
* TODO: Fix. Now partial order is broken. Need to expand subroots and propagate changes down the tree.
*/
void Node::Insert(const Set& new_set) {


    // Step -1: check if leaf already exists
    if (new_set == set && IsLeaf()) {
        return;
    }
    Set orig_set = set;

    // Step 0: check if Node has no children.

    if (subnodes[0] == nullptr) {
        // No subnodes at all. Just add the first subnode.
        subnodes[0] = new Node(this, new_set);
        return;
    }

    // Step 1: find MIN{new_set \ subnode[i]}
    //    It will show, how much do we need to expand a node to fit the new_set in it

    short argmin_i;
    Set complements[2] = {
        new_set.Minus(subnodes[0]->set),
        ( subnodes[1] != nullptr ) ? new_set.Minus(subnodes[1]->set) : Set(), 
    };
    size_t complement_sizes[2] = {
        complements[0].Size(),
        complements[1].Size()
    };

    unsigned int subset_nodes = subset_test(new_set);

    // Step 2: Subset test (check if new_set <= A)
    //     because (|X\A| = 0) => (X is a subset of A)

    // Note: There can be a situation when new_set <= subnodes[0] AND new_set <= subnodes[1].
    //   In such case just choose left subtree. 
    //   It is ok because subset test always starts from the subnode[0].

    // Firstly: if new_set is not a subset nor superset of subnodes[0], just insert it as a subnode[1]


    if (complement_sizes[0] == 0) {
        // new_set <= subnodes[0]
        // Insert in 0th subtree.
        subnodes[0]->Insert(new_set);
        return;
    } 

    if (subset_nodes == 1) {
        // subnode[0] <= new_set
        // Insert as new root of the 0th subtree

        Node* new_node = new Node(this, new_set);
        new_node->subnodes[0] = subnodes[0];
        subnodes[0]->parent = new_node;
        subnodes[0] = new_node;
        return;
    }

    // Now new_set is not a subset nor superset of subnodes[0]

    if (subnodes[1] == nullptr) {
        // just insert as a new second subnode.
        subnodes[1] = new Node(this, new_set);
        return;
    }

    // Now subnodes[1] != nullptr !

    // Other checks:

    if (complement_sizes[1] == 0) {
        // new_set <= subnode[1]
        subnodes[1]->Insert(new_set);
        return;
    }
    
    if (subset_nodes == 3) {
        // subnode[0], subnode[1] both are subsets of new_set

        /*  
        FROM

        parent 
        +-- this(set) 
            +-- node[1]
            +-- node[2]  
        +-- other

        TO

        parent 
        +-- this(set) 
            +-- new_node(new_set) 
                +-- node[1]
                +-- node[2]  
            
        +-- other
        */
        
        Node* new_node = new Node(this, new_set);
        new_node->subnodes[0] = subnodes[0];
        new_node->subnodes[1] = subnodes[1];
        subnodes[0]->parent = new_node;
        subnodes[1]->parent = new_node;
        subnodes[0] = new_node;
        subnodes[1] = nullptr;
        return;
    }

    if (subset_nodes == 2) {
        // subnode[1] <= set
        // same as in previous `if`, but only for subnode[1]

        Node* new_node = new Node(this, new_set);
        new_node->subnodes[1] = subnodes[1];
        subnodes[1]->parent = new_node;
        subnodes[1] = new_node;
        return;
    }

    // Step 4: (otherwise) General insert

    if (complement_sizes[0] <= complement_sizes[1]) {
        argmin_i = 0;
    } else {
        argmin_i = 1;
    }

    subnodes[argmin_i]->Insert(new_set);
}



/* Returns a bitmask of subnodes, where `set` does fit:
*    0 = 00b -  No subnode is a SUPERSET of `set`
*    1 = 01b - set <= subnode[0]
*    2 = 10b - set <= subnode[1]
*    3 = 11b - set <= subnode[0], subnode[1]
*/
unsigned int Node::superset_test(const Set& set) const {
    unsigned int mask = 0;
    if (subnodes[0] && set.IsSubsetOf(subnodes[0]->set)) {
        mask |= 1;
    }
    if (subnodes[1] && set.IsSubsetOf(subnodes[1]->set)) {
        mask |= 2;
    }
    return mask;
}

/* Returns a bitmask of subnodes with subsets of `set`:
*    0 = 00b -  No subnode is a SUBSET of `set`
*    1 = 01b - subnode[0] <= set
*    2 = 10b - subnode[1] <= set
*    3 = 11b - subnode[0], subnode[1] <= set 
*/
unsigned int Node::subset_test(const Set& set) const {
    unsigned int mask = 0;
    if (subnodes[0] && subnodes[0]->set.IsSubsetOf(set)) {
        mask |= 1;
    }
    if (subnodes[1] && subnodes[1]->set.IsSubsetOf(set)) {
        mask |= 2;
    }
    return mask;
}
