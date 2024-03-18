#ifndef FACTORY_HPP
# define FACTORY_HPP

# include <unordered_map>
# include <functional>
# include <utility>
# include <memory>

template <typename Key, typename Product>
class Factory {
public:
	typedef std::function<std::unique_ptr<Product>(std::string)> Factor;

	static void registerFactory(std::string key, Factor factor) {
		auto it = factories_.find(key);
		if (it == factories_.end()) {
			factories_.insert(std::make_pair(key, factor));
		}
	}

	static std::unique_ptr<Product> createProduct(const Key& key) {
		auto it = factories_.find(key.first);
		if (it != factories_.end()) {
			return it->second(key.second);
		}
		return nullptr;
	
	}
private:
	static std::unordered_map<std::string, Factor> factories_;
};

template <typename Key, typename Product>
std::unordered_map<std::string, typename Factory<Key, Product>::Factor> Factory<Key, Product>::factories_;

#endif
