#include <vector>

#include "shim.h"

typedef std::vector<Color> ColorVec;

class Ws281xStringEmu : public Ws281xString
{
public:
  Ws281xStringEmu(ColorVec&);
  void set(u16 pixelIdx, const Color&);
  Color get(u16 pixelIdx);
  u16 len();
private:
  ColorVec& leds;
};
