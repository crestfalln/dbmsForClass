#include <unordered_map>
#include <iostream>
#include <future>
#include <mutex>
#include <vector>
#include <thread>

std::mutex m;
std::vector<std::future<void>> futures;

void map_delete(std::unordered_multimap<int ,int>::iterator iter , std::unordered_multimap<int,int>& map)
{
	m.lock();
	map.erase(iter);
	std::cout << "Erased" << std::endl;
	m.unlock();
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void map_iter(std::unordered_multimap<int ,int>::iterator iter , std::unordered_multimap<int,int> &map)
{
	for(auto &iter : map)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << iter.first << ' ' << iter.second << std::endl;
	}
}

int main()
{
	std::unordered_multimap<int ,int> map;
	map.insert({{12 , 2 } , 
	{12 , 6 },
	{12 , 4 },
	{12 , 8 },
	{892 , 2 },
	{892 , 12 }
	});
	futures.push_back(std::async(std::launch::async , map_iter, map.begin() , std::ref(map)));
	futures.push_back(std::async(std::launch::async , map_delete, map.begin() , std::ref(map)));
}