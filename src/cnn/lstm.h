#ifndef CNN_LSTM_H_
#define CNN_LSTM_H_

#include "cnn/cnn.h"
#include "cnn/edges.h"

namespace cnn {

class Model;

struct LSTMBuilder {
  LSTMBuilder() {}
  explicit LSTMBuilder(unsigned layers,
                       unsigned input_dim,
                       unsigned hidden_dim,
                       Model* model);

  // call this to reset the builder when you are going to create
  // a new computation graph
  void new_graph();

  // call this before add_input
  void add_parameter_edges(Hypergraph* hg);

  // add another timestep by reading in the variable x
  // return the hidden representation of the deepest layer
  VariableIndex add_input(VariableIndex x, Hypergraph* hg);

  // rewind the last timestep - this DOES NOT remove the variables
  // from the computation graph, it just means the next time step will
  // see a different previous state. You can remind as many times as
  // you want.
  void rewind_one_step() {
    h.pop_back();
    c.pop_back();
  }

  // returns node index (variable) of most recent output
  VariableIndex back() const { return h.back().back(); }

  // check to make sure parameters have been added before adding input
  unsigned builder_state;

  // first index is layer, then ...
  std::vector<std::vector<Parameters*>> params;

  // first index is layer, then ...
  std::vector<std::vector<VariableIndex>> param_vars;

  // first index is time, second is layer 
  std::vector<std::vector<VariableIndex>> h, c;

  unsigned layers;
};

} // namespace cnn

#endif
