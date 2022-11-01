#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>

using namespace std;

struct Point
{
 int x;
 int y;
};

void print_vector(vector<float> v)
{
    copy(v.begin(), v.end(), ostream_iterator<float>(cout, " "));
    cout << endl;
}

float get_hypot(Point point1, Point point2)
{
    float result = 0;
    result += pow((point1.x - point2.x), 2);
    result += pow((point1.y - point2.y), 2);
    result = pow(result, 0.5);
    return result;
}

int main()
{
  vector<Point> points = {
      {1, 2}, 
      {3, 4}, 
      {5, 5}
  };
  
  vector<float> lengths_difference = {};
  for (int i = 0; i < points.size() - 1; ++i)
  {
      lengths_difference.push_back(get_hypot(points[i], points[i + 1]));
  }
  
  adjacent_difference(lengths_difference.begin(), lengths_difference.end(), lengths_difference.begin(), plus<int>{});  
  print_vector(lengths_difference);
}