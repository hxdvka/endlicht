#include <random>

#define QSIZE 25

//reinvent the wheel but like <mini>
//btw there should be a cxx or  hxx
// TODO update to template
struct aux {
  uint R;
  uint G;
  uint B;
  uint d;
  aux()
    : R{ 1 },
      G{ 20 },
      B{ 300 },
      d{ 0 } 
      {}
  void set(uint R, uint G, uint B, uint d) {
    this->R = R;
    this->G = G;
    this->B = B;
    this->d = d;
  }
  //Arduino String lib bc stuff happens . -.
  String toStr() {
    String ret;
    ret += " R G B d:\n";
    ret += String(R) +" : "+ String(G) +" : "+ String(B) +" : "+ String(d);
    return ret;
  }
};

struct miniQ {
  uint erste;
  uint count;
  aux* qQ;

  //this is redundant will figure out constructors later
  miniQ()
    : erste{ 0 },
      count{ 0 },
      qQ{ new aux[25]() } {
    erste = 0;
    count = 0;
    this->qQ = new aux[25]();
  }

  //randoom data is love random data is life
  bool addRand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint> dis(0, 255);
    return this->add(dis(gen), dis(gen), dis(gen), dis(gen));
  }

  //new cmds will be /dropped/ when full
  bool add(uint R, uint G, uint B, uint d) {
    bool ret = 0;
    if (hasRoom()) {
      qQ[(erste + count) % QSIZE].set(R, G, B, d);
      count++;
      ret = 1;
    }
    return ret;
  }
  //do i need to check for overflow? didn't care but DONE
  bool pop(aux &data) {
    bool ret = 0;
    if (count) { // same as !isEmpty()
      data = (qQ[erste]);
      erste++;
      count--;
      erste %= QSIZE;
      ret = 1;
    } 
    return ret;
  }

  bool hasRoom() {
    return QSIZE > count;
  }

  bool isEmpty() {
    return !count;
  }

  void status(int i = -1) {
    printf("\nerste count i\t %d : %d : %d\n", erste, count, i);
  }
};
