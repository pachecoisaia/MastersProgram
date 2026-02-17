#ifndef configurable_h
#define configurable_h

// Interface for configurable components
class Configurable {
public:
  virtual ~Configurable() {}
  virtual void configure() = 0;
};

#endif
