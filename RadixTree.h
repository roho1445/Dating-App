#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <string>
//Represent Values
template <typename ValueType>
class ValueNode {
public:

	ValueNode() {}

	ValueNode(const ValueType& passedValue) : value(passedValue) {
	}

	~ValueNode() { }

	void setValue(const ValueType& passedValue) {
		value = passedValue;
	}

	ValueType* getValue() {
		return &value;
	}
private:
	ValueType value;
};

//Represent TreeNodes
template <typename ValueType>
class TreeNode {
public:
	TreeNode() {
		key = std::string("");
	}

	TreeNode(std::string passedkey, const ValueType& passedValue) {
		key = passedkey;
		value = new ValueNode<ValueType>(passedValue);
	}

	~TreeNode() {
		if (branches) {
			delete[] branches;
		}

		if (value) {
			delete value;
		}
	}

	std::string getKey() {
		return key;
	}

	void setKey(std::string newKey) {
		key = newKey;
	}

	ValueType* getValue() {
		if (value) {
			return value->getValue();
		}
		return nullptr;
	}

	void setValue(const ValueType& newValue) {
		if (value) {
			value->setValue(newValue);
		}
		else {
			value = new ValueNode<ValueType>(newValue);
		}
	}

	void setValueNode(ValueNode<ValueType>* newNode) {
		value = newNode;
	}

	void moveValue(TreeNode<ValueType>* node) {
		node->setValueNode(value);
		value = nullptr;
	}

	TreeNode<ValueType>** getBranches() {
		return branches;
	}

	void setBranches(TreeNode<ValueType>** newBranches) {
		branches = newBranches;
	}

	void moveBranches(TreeNode<ValueType>* node) {
		node->setBranches(branches);
		branches = nullptr;
	}

	void addBranch(const char& c, TreeNode* newNode) {
		if (!branches) {
			branches = new TreeNode<ValueType>*[128];
			for (int i = 0; i < 128; i++) {
				branches[i] = nullptr;
			}
		}
		int index = static_cast<int>(c);
		branches[index] = newNode;
		//branches[c] = newNode;
		return;
	}

	TreeNode<ValueType>* getBranchNode(const char& c) {
		if (branches) {
			int index = static_cast<int>(c);
			return branches[index];
			//return branches[c];
		}
		return nullptr;
	}
private:
	std::string key;

	ValueNode<ValueType>* value;

	TreeNode<ValueType>** branches;
};

template <typename ValueType>
class RadixTree {
public:
	RadixTree() :root(nullptr) {
	}

	~RadixTree() {
		if (root) {
			delete root;
		}
	}

	void insert(std::string insertKey, const ValueType& value) {

		if (insertKey.empty()) {
			return;
		}

		if (!root) {
			root = new TreeNode<ValueType>(insertKey, value);
		}
		else {
			TreeNode<ValueType>* node = root;
			std::string key = insertKey;
			while (node) {
				int pos = getMatchedPosition(node, key);

				if (pos != -1) {
					if (pos == node->getKey().length() - 1 && pos == key.length() - 1) {
						// key matches exactly the node value
						node->setValue(value);
						return;
					}
					else if (pos == node->getKey().length() - 1 && pos < key.length() - 1) {
						// matched position less than key: insert node for key[pos+1] onwards
						char branchPos = key.at(pos + 1);
						std::string newkey = getRemainingKey(key, pos);

						TreeNode<ValueType>* newNode = node->getBranchNode(branchPos);
						if (newNode) {
							node = newNode;
							key = newkey;
						}
						else {
							newNode = new TreeNode<ValueType>(newkey, value);
							node->addBranch(branchPos, newNode);
							return;
						}
					}
					else if (pos < node->getKey().length() - 1 && pos == key.length() - 1) {
						// key is shorter than node string: split node into key(pos) and node->getKey()[pos+1]
						char branchPos = node->getKey().at(pos + 1);
						std::string newKey = getRemainingKey(node->getKey(), pos);

						TreeNode<ValueType>* newNode = new TreeNode<ValueType>();
						newNode->setKey(newKey);
						node->moveValue(newNode);
						node->moveBranches(newNode);
						node->addBranch(branchPos, newNode);
						node->setKey(key);
						node->setValue(value);
						return;
					}
					else if (pos < node->getKey().length() - 1 && pos < key.length() - 1) {
						// matched position is less than both key and node string
						char branchPos = key.at(pos + 1);
						TreeNode<ValueType>* newNode = node->getBranchNode(branchPos);
						if (newNode) {
							node = newNode;
							key = getRemainingKey(key, pos);
						}
						else {
							splitNode(node, key, value, pos);
							return;
						}
					}
				}
				else {  // search the branches directly
					if (node->getKey().length() > 0) {  // unmatched key
						splitNode(node, key, value, -1);
						return;
					}
					else {
						char branchPos = key.at(pos + 1);
						std::string newkey = getRemainingKey(key, pos);

						TreeNode<ValueType>* nextNode = node->getBranchNode(branchPos);
						if (nextNode) {
							node = nextNode;
							key = newkey;
						}
						else {
							TreeNode<ValueType>* newNode = new TreeNode<ValueType>(newkey, value);
							node->addBranch(branchPos, newNode);
							return;
						}
					}
				}
			}
		}
	}

	ValueType* search(std::string key) const {

		if (!key.empty()) {
			std::string currentKey = key;
			TreeNode<ValueType>* node = root;
			while (node) {
				int pos = getMatchedPosition(node, currentKey);

				if (pos == node->getKey().length() - 1 && pos == currentKey.length() - 1) {
					return node->getValue();
				}

				if (pos == -1 && node->getKey().length() > 0) {  // unmatched node entry
					node = nullptr;
				}
				else if (pos == -1 || pos < currentKey.length() - 1) {
					char branchPos = currentKey.at(pos + 1);
					node = node->getBranchNode(branchPos);
					currentKey = getRemainingKey(currentKey, pos);
				}
				else {
					node = nullptr;
				}
			}
		}
		return nullptr;
	}
private:
	TreeNode<ValueType>* root;

	std::string getRemainingKey(const std::string& currentKey, int pos) const {
		std::string newKey{};
		if (pos + 1 < currentKey.length() - 1) {
			newKey = currentKey.substr(pos + 2);
		}
		return newKey;
	}

	int getMatchedPosition(TreeNode<ValueType>* node, const std::string& key) const {
		int pos = -1;
		for (int i = 0; i < node->getKey().length() && i < key.length(); ++i) {
			if (node->getKey().at(i) == key.at(i)) {
				pos = i;
			}
			else {
				break;
			}
		}
		return pos;
	}

	void splitNode(TreeNode<ValueType>* node, std::string key, const ValueType& value, int pos) {

		char branchPos = node->getKey().at(pos + 1);
		std::string newkey = getRemainingKey(node->getKey(), pos);
		TreeNode<ValueType>* newNode = new TreeNode<ValueType>();
		newNode->setKey(newkey);
		node->moveValue(newNode);
		node->moveBranches(newNode);

		node->setKey(key.substr(0, pos + 1));
		node->addBranch(branchPos, newNode);

		branchPos = key.at(pos + 1);
		newkey = getRemainingKey(key, pos);
		newNode = new TreeNode<ValueType>(newkey, value);
		node->addBranch(branchPos, newNode);

		return;
	}
};

#endif