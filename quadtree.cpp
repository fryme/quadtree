#include <iostream>
#include <memory>
#include <list>
#include <vector>
#include <stdint.h>
#include <array>

struct Point
{
	int32_t x;
	int32_t y;

	Point() : x(0), y(0) {}
	Point(int32_t x_, int32_t y_) : x(x_), y(y_) {}
};

struct Rectangle
{
public:

	Rectangle() {}

	Rectangle(Point& tl, uint32_t width_, uint32_t height_) : m_topLeft(tl), m_widht(width_), m_height(height_)
	{
	}

	bool Contains(const Point& p) const
	{
		return (GetX() < p.x && 
				GetY() < p.y &&
				GetX() + GetWidth() > p.x &&
				GetY() + GetHeight() > p.y);
	}

	void SetXY(const Point& p)
	{
		m_topLeft.x = p.x;
		m_topLeft.y = p.y;
	}
	
	void SetWidth(uint32_t width)
	{
		m_widht = width;
	}

	void SetHeight(uint32_t height)
	{
		m_height = height;
	}

	uint32_t GetWidth() const
	{
		return m_widht;
	}

	uint32_t GetHeight() const
	{
		return m_height;
	}

	int32_t GetX() const
	{
		return m_topLeft.x;
	}

	int32_t GetY() const
	{
		return m_topLeft.y;
	}

	bool Intersects(const Rectangle& other) const
	{
		return (this->GetX() < (other.GetX() + other.GetWidth()) && 
				(this->GetX() + GetWidth()) > other.GetX() &&
				this->GetY() < (other.GetY() + other.GetHeight()) && 
				(this->GetY() + GetHeight()) > other.GetY());
	}

private:
	Point m_topLeft;
	uint32_t m_widht, m_height;
};

class QuadTree
{
	const int NODE_CAPACITY = 4;
public:
	QuadTree(Rectangle& bounds) : m_bounds(bounds), m_nodes({{ nullptr, nullptr, nullptr, nullptr }})
	{
	}

	bool Insert(const Point& point)
	{
		if (!m_bounds.Contains(point))
			return false;

		if (m_objects.size() < NODE_CAPACITY)
		{
			m_objects.push_back(point);
			return true;
		}
		
		if (m_nodes[0] == nullptr) 
			split();

		if (m_nodes[0]->Insert(point)) return true;
		if (m_nodes[1]->Insert(point)) return true;
		if (m_nodes[2]->Insert(point)) return true;
		if (m_nodes[3]->Insert(point)) return true;

		return false;
	}

	void QueryRange(const Rectangle& range, std::vector<Point>& pointsInRange) const
	{
		if (!m_bounds.Intersects(range))
			return;

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (range.Contains(*it))
				pointsInRange.push_back(*it);
		}

		if (m_nodes[0] == nullptr)
			return;

		// Otherwise, add the points from the children
		m_nodes[0]->QueryRange(range, pointsInRange);
		m_nodes[1]->QueryRange(range, pointsInRange);
		m_nodes[2]->QueryRange(range, pointsInRange);
		m_nodes[3]->QueryRange(range, pointsInRange);
	}

	void UpdatePoint(const Point&)
	{
		// LOCKS!
		// UpdateByName?
	}

private:

	void split()
	{
		const uint32_t halfWidth = m_bounds.GetWidth() / 2;
		const uint32_t halfHeight = m_bounds.GetHeight() / 2;
		const uint32_t x = (int)m_bounds.GetX();
		const uint32_t y = (int)m_bounds.GetY();

		m_nodes[0] = new QuadTree(Rectangle(Point(x + halfWidth, y), halfWidth, halfHeight));
		m_nodes[1] = new QuadTree(Rectangle(Point(x, y), halfWidth, halfHeight));
		m_nodes[2] = new QuadTree(Rectangle(Point(x, y + halfHeight), halfWidth, halfHeight));
		m_nodes[3] = new QuadTree(Rectangle(Point(x + halfWidth, y + halfHeight), halfWidth, halfHeight));
	}

	std::list<Point>		m_objects;
	Rectangle				m_bounds;
	std::array<QuadTree*, 4>  m_nodes;
};

#include <fstream>
#include <sstream>

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

void read_from_file(const std::wstring& filepath, std::vector<Point>& arr, Rectangle& rectangle)
{
	std::ifstream input(filepath);
	std::string str((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());
	
	arr.clear();
	vector<string> x = split(str, ';');
	for (int index = 0; index < x.size(); ++index)
	{
		vector<string> x2;
		std::string& pointStr = x[index];
		const auto str = pointStr.substr(1, pointStr.size() - 1);
		x2 = split(str, ',');
		if (x2.size() == 2)
		{
			arr.push_back(Point(std::stoi(x2[0]), std::stoi(x2[1])));
		}
		else
		{
			rectangle.SetXY(Point(std::stoi(x2[0]), std::stoi(x2[1])));
			rectangle.SetHeight(std::stoi(x2[2]));
			rectangle.SetWidth(std::stoi(x2[3]));
		}
	}
}
#include <ctime>
#include <iomanip>

template <typename T>
void Log(T data)
{
	std::time_t t = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &t);
	cout << std::put_time(&tm, "%H:%M:%S: ") << data << std::endl;
}

int main()
{
//	std::cout.imbue(std::locale("ru_RU.utf8"));
	std::vector<Point> arr;
	Rectangle rect;
	Log("Reading file");
	read_from_file(L"F:\\dev\\github\\find_nearest\\out.txt", arr, rect);

	time_t  timev;
	time(&timev);
	Log("Begin creating tree");
	std::shared_ptr<QuadTree> qtree = std::make_shared<QuadTree>(Rectangle(Point(0,0), 1000, 1000));
	for (auto point : arr)
		qtree->Insert(point);

	Log("Insert completed");

	std::vector<Point> pointsInRange;
	qtree->QueryRange(rect, pointsInRange);

	Log("Query completed");


	/*
	qtree->Insert(Point(1, 1));
	qtree->Insert(Point(33, 33));
	qtree->Insert(Point(55, 55));
	qtree->Insert(Point(33, 44));
	qtree->Insert(Point(6, 44));
	qtree->Insert(Point(8, 12));
	*/

	std::vector<Point> checker;
	for (auto point : arr)
	{
		if (rect.Contains(point))
			checker.push_back(point);
	}
	Log("Finish");
	return 0;
}