#ifndef EX3__MAPOBJ_H_
#define EX3__MAPOBJ_H_
class MapObj{
 private:
  double value;

 public:
  MapObj(double val){
    set(val);
  }
  void set(double val){
    this->value = val;
  }
  double get(){
    return this->value;
  }
};
#endif //EX3__MAPOBJ_H_
