#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>


using std::function;
using std::make_pair;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::weak_ptr;


template<typename Asset>
class AssetCache {
public:
	AssetCache();
	AssetCache(function<Asset* (const string& filename)> loader);
	shared_ptr<const Asset> load(const string& filename);


private:
	function<Asset* (const string& filename)> loader;
	unordered_map<string, weak_ptr<Asset>> assets;
};


template<typename Asset>
AssetCache<Asset>::AssetCache() {
	loader = [](const string& filename) {
		return new Asset(filename);
	};
}


template<typename Asset>
AssetCache<Asset>::AssetCache(function<Asset* (const string& filename)> loader) {
	this->loader = loader;
}


template<typename Asset>
shared_ptr<const Asset> AssetCache<Asset>::load(const string& filename) {
	auto it = assets.find(filename);
	if (it == assets.end()) {
		auto deleter = [this, filename](Asset* asset) {
			assets.erase(filename);
			delete asset;
		};
		auto asset = shared_ptr<Asset>(loader(filename), deleter);
		assets.insert(make_pair(filename, asset));
		return asset;
	} else {
		return shared_ptr<Asset>(it->second);
	}
}
