typedef unsigned int size_t;

struct DDS_Duration_t
{
  size_t _min;
  size_t _max;

  DDS_Duration_t(size_t min, size_t max) : _min(min), _max(max) {}
};

int main(void)
{
  DDS_Duration_t wait1 = {1, 0};
  DDS_Duration_t wait3(1, 0);
}

