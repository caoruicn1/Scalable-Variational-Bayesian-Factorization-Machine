// Author Avijit Saha
// This file contains exponential family stochastic gradient descent
//


#ifndef EXP_FM_LEARN_SGD_STOC_H_
#define EXP_FM_LEARN_SGD_STOC_H_

#include "fm_learn.h"
#include "../../fm_core/fm_sgd.h"

class exp_fm_learn_sgd_stoc: public fm_learn {
	protected:
		//DVector<double> sum, sum_sqr;
	public:
		int num_iter;
		double learn_rate;
		DVector<double> learn_rates;

		virtual void init() {
			fm_learn::init();
			learn_rates.setSize(3);
		//	sum.setSize(fm->num_factor);
		//	sum_sqr.setSize(fm->num_factor);
		}

		virtual void learn(DataSubset& train, DataSubset& test) {
			fm_learn::learn(train, test);
			std::cout << "learnrate=" << learn_rate << std::endl;
			std::cout << "learnrates=" << learn_rates(0) << "," << learn_rates(1) << "," << learn_rates(2) << std::endl;
			std::cout << "#iterations=" << num_iter << std::endl;

			if (train.relation.dim > 0) {
				throw "relations are not supported with SGD";
			}
			std::cout.flush();
		}

		void SGD(sparse_row<DATA_FLOAT> &x, const double multiplier, DVector<double> &sum) {
			fm_SGD(fm, learn_rate, x, multiplier, sum);
		}

		void debug() {
			std::cout << "num_iter=" << num_iter << std::endl;
			fm_learn::debug();
		}

		virtual void predict(DataSubset& data, DVector<double>& out) {
			assert(data.data->getNumRows() == out.dim);
			for (data.data->begin(); !data.data->end(); data.data->next()) {
				double p = predict_case(data);
				if (task == TASK_REGRESSION ) {
					p = std::min(max_target, p);
					p = std::max(min_target, p);
					//p=p;
				} else if (task == TASK_CLASSIFICATION) {
					p = 1.0/(1.0 + exp(-p));
				} else {
					throw "task not supported";
				}
				out(data.data->getRowIndex()) = p;
			}
		}

};

#endif /*FM_LEARN_SGD_H_*/
