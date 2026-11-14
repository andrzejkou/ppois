#include "Evaluation.h"

#include <stdexcept>
#include <utility>

Evaluation::Evaluation(std::string evaluatorId, std::string comment)
    : evaluatorId_(std::move(evaluatorId)),
      comment_(std::move(comment)) {
  if (evaluatorId_.empty()) {
    throw std::invalid_argument("Evaluator ID cannot be empty");
  }
}

void Evaluation::reviseComment(const std::string &comment) {
  if (comment.empty()) {
    throw std::invalid_argument("Evaluation comment cannot be empty");
  }

  comment_ = comment;
}

const std::string &Evaluation::evaluatorId() const {
  return evaluatorId_;
}

const std::string &Evaluation::comment() const {
  return comment_;
}
