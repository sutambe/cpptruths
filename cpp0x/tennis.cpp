#include <iostream>
#include <cstdlib>
#include <variant>
#include <string>

template<class... Ts> struct overloaded : Ts... { 
    using Ts::operator()...; 
    explicit overloaded(Ts... ts) : Ts(ts)... {}
};
//template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

struct Player {
  std::string name;   
  bool operator == (const Player& other) const { return name == other.name; }
};

std::ostream& operator << (std::ostream& o, const Player& p) {
  o << p.name;
  return o;
};

struct NormalScore {
  Player p1, p2;
  int p1_score, p2_score;
};

struct DeuceScore {
  Player p1, p2;  
};

struct AdvantageScore {
  Player lead, lagging;  
};

struct GameCompleteScore {
  Player winner, loser;
  int loser_score;
};

typedef std::variant<NormalScore, DeuceScore, AdvantageScore, GameCompleteScore> GameState;

GameState next(const GameState& now, const Player& who_scored) {
  return
    std::visit(overloaded {
                [&](const NormalScore& ns) -> GameState { 
                  if (ns.p1 == who_scored) { 
                    switch (ns.p1_score) {
                      case  0: return NormalScore{ns.p1, ns.p2, 15, ns.p2_score}; 
                      case 15: return NormalScore{ns.p1, ns.p2, 30, ns.p2_score}; 
                      case 30: if (ns.p2_score < 40) 
                                 return NormalScore{ns.p1, ns.p2, 40, ns.p2_score};
                               else
                                 return DeuceScore{ns.p1, ns.p2};
                      case 40: return GameCompleteScore{ns.p1, ns.p2, ns.p2_score};
                      default: throw "Makes no sense!";
                    }
                  }
                  else {
                    switch (ns.p2_score) {
                      case  0: return NormalScore{ns.p1, ns.p2, ns.p1_score, 15}; 
                      case 15: return NormalScore{ns.p1, ns.p2, ns.p1_score, 30}; 
                      case 30: if (ns.p1_score < 40) 
                                 return NormalScore{ns.p1, ns.p2, ns.p1_score, 40};
                               else
                                 return DeuceScore{ns.p1, ns.p2};
                      case 40: return GameCompleteScore{ns.p2, ns.p1, ns.p1_score};
                      default: throw "Makes no sense!"; 
                    }
                  }
                }, 
                [&](const DeuceScore& ds) -> GameState { 
                    if (ds.p1 == who_scored) 
                      return AdvantageScore{ds.p1, ds.p2};
                    else
                      return AdvantageScore{ds.p2, ds.p1};
                },
                [&](const AdvantageScore& as) -> GameState { 
                    if (as.lead == who_scored) 
                      return GameCompleteScore{as.lead, as.lagging, 40};
                    else
                      return DeuceScore{as.lead, as.lagging};
                },
                [&](const GameCompleteScore &) -> GameState { throw "Illegal State"; }
             }, now);
}

void next_mutable(GameState& now, const Player& who_scored) {
      std::visit(overloaded {
                [&](const NormalScore& ns) { 
                  if (ns.p1 == who_scored) { 
                    switch (ns.p1_score) {
                      case  0: now = NormalScore{ns.p1, ns.p2, 15, ns.p2_score}; break;
                      case 15: now = NormalScore{ns.p1, ns.p2, 30, ns.p2_score};  break;
                      case 30: if (ns.p2_score < 40) 
                                 now = NormalScore{ns.p1, ns.p2, 40, ns.p2_score};  
                               else
                                 now = DeuceScore{ns.p1, ns.p2}; 
                               break;
                      case 40: now = GameCompleteScore{ns.p1, ns.p2, ns.p2_score};break;
                      default: throw "Makes no sense!";
                    }
                  }
                  else {
                    switch (ns.p2_score) {
                      case  0: now = NormalScore{ns.p1, ns.p2, ns.p1_score, 15}; break;
                      case 15: now = NormalScore{ns.p1, ns.p2, ns.p1_score, 30}; break;
                      case 30: if (ns.p1_score < 40) 
                                 now = NormalScore{ns.p1, ns.p2, ns.p1_score, 40};
                               else
                                 now = DeuceScore{ns.p1, ns.p2};
                               break;
                      case 40: now = GameCompleteScore{ns.p2, ns.p1, ns.p1_score};break;
                      default: throw "Makes no sense!"; 
                    }
                  }
                }, 
                [&](const DeuceScore& ds) { 
                    if (ds.p1 == who_scored) 
                      now = AdvantageScore{ds.p1, ds.p2};
                    else
                      now = AdvantageScore{ds.p2, ds.p1};
                },
                [&](const AdvantageScore& as) { 
                    if (as.lead == who_scored) 
                      now = GameCompleteScore{as.lead, as.lagging, 40};
                    else
                      now = DeuceScore{as.lead, as.lagging};
                },
                [&](const GameCompleteScore &) { throw "Illegal State"; }
             }, now);
}

struct GameStatePrinter {
        std::ostream &o;
        explicit GameStatePrinter(std::ostream& out) : o(out) {}
    
        void operator ()(const NormalScore& ns) const { 
          o << "NormalScore[" << ns.p1 << "," << ns.p2 << "," << ns.p1_score << "," << ns.p2_score << "]";
        }
    
        void operator ()(const DeuceScore& ds) const {
          o << "DeuceScore[" << ds.p1 << "," << ds.p2 << "]";
        }
    
        void operator ()(const AdvantageScore& as) const {
          o << "AdvantageScore[" << as.lead << "," << as.lagging << "]";
        }
    
        void operator ()(const GameCompleteScore& gc) const { 
            o << "GameComplete[" << gc.winner << "," << gc.loser << "," << gc.loser_score << "]";
        } 
};

std::ostream & operator << (std::ostream& o, const GameState& game) {
  std::visit(GameStatePrinter(o), game);
  std::visit(
      overloaded {
        [&](const NormalScore& ns) { 
          o << "NormalScore[" << ns.p1 << "," << ns.p2 << "," << ns.p1_score << "," << ns.p2_score << "]";
        },
        [&](const DeuceScore& ds) {
          o << "DeuceScore[" << ds.p1 << "," << ds.p2 << "]";
        },
        [&](const AdvantageScore& as) {
          o << "AdvantageScore[" << as.lead << "," << as.lagging << "]";
        },
        [&](const GameCompleteScore& gc) { 
            o << "GameComplete[" << gc.winner << "," << gc.loser << "," << gc.loser_score << "]";
        }
      }, game);
  return o;
}


int main()
{
  Player p1{"Nadal"}, p2{"Federer"};
  GameState g1 = NormalScore{p1, p2, 0, 0};
  GameState g2 = next(next(next(g1, p1), p1), p1);
  GameState g3 = next(next(next(next(next(g2, p2), p2), p2), p2), p2);
  std::cout << g3 << "\n";
    
  GameState g4 = NormalScore{p1, p2, 0, 0};
  next_mutable(g4, p1);
  next_mutable(g4, p1);
  next_mutable(g4, p1);
  next_mutable(g4, p1);
  std::cout << g4 << "\n";

}
