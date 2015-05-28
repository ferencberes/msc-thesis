#include "centrality_utils.h"

using namespace lemon;

void IdValuePair::str() {
	std::cout << "id: " << id << ", score: " << value << std::endl;
}

inline bool pair_comparator::operator()(const IdValuePair& struct1,
		const IdValuePair& struct2) {
	return (struct1.value > struct2.value);
}

void TopKSelector::push(IdValuePair newValue) {
	if (vec.size() == k_max_size) {
		if (vec.back().value < newValue.value) {
			vec.pop_back();
			vec.push_back(newValue);
			sort();
		}
	} else {
		vec.push_back(newValue);
		sort();
	}
}

void TopKSelector::sort() {
	std::sort(vec.begin(), vec.end(), pair_comparator());
}

unsigned int TopKSelector::size() const {
	return k_max_size;
}

const std::vector<IdValuePair>& TopKSelector::getVector() const {
	return vec;
}

void salsa_data::setAuthorityValue(float value) {
	authority = value;
}

void salsa_data::setHubValue(float value) {
	hub = value;
}
