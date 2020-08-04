#include <iostream>
#include <time.h>
#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/include/async.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/parallel_generate.hpp>
#include <hpx/include/parallel_sort.hpp>
// OpenNN includes

#include "/home/ashwath/Desktop/opennn/opennn/opennn/opennn.h"

using namespace OpenNN;

void print(Vector<double> binary_classification_tests)
{
	hpx::cout << "Binary classification tests: " << endl;
        hpx::cout << "Classification accuracy         : " << binary_classification_tests[0] << hpx::endl;
        hpx::cout << "Error rate                      : " << binary_classification_tests[1] << hpx::endl;
        hpx::cout << "Sensitivity                     : " << binary_classification_tests[2] << hpx::endl;
        hpx::cout << "Specificity                     : " << binary_classification_tests[3] << hpx::endl;
        hpx::cout << "Precision                       : " << binary_classification_tests[4] << hpx::endl;
        hpx::cout << "Positive likelihood             : " << binary_classification_tests[5] << hpx::endl;
        hpx::cout << "Negative likelihood             : " << binary_classification_tests[6] << hpx::endl;
        hpx::cout << "F1 score                        : " << binary_classification_tests[7] << hpx::endl;
        hpx::cout << "False positive rate             : " << binary_classification_tests[8] << hpx::endl;
        hpx::cout << "False discovery rate            : " << binary_classification_tests[9] << hpx::endl;
        hpx::cout << "False negative rate             : " << binary_classification_tests[10] << hpx::endl;
        hpx::cout << "Negative predictive value       : " << binary_classification_tests[11] << hpx::endl;
        hpx::cout << "Matthews correlation coefficient: " << binary_classification_tests[12] << hpx::endl;
        hpx::cout << "Informedness                    : " << binary_classification_tests[13] << hpx::endl;
        hpx::cout << "Markedness                      : " << binary_classification_tests[14] << hpx::endl;
}
	

int main(void)
{
    try
    {
        hpx::cout << "OpenNN. Breast Cancer Application." << hpx::endl;

        srand(static_cast<unsigned>(time(nullptr)));

        // Data set

        DataSet data_set("/home/ashwath/Desktop/opennn/examples/breast_cancer/data/breast_cancer.csv",';',true);

        data_set.split_instances_random();

        const Vector<string> inputs_names = data_set.get_input_variables_names();
        const Vector<string> targets_names = data_set.get_target_variables_names();

        const Vector<Descriptives> inputs_descriptives = data_set.scale_inputs_minimum_maximum();

        // Neural network

        NeuralNetwork neural_network(NeuralNetwork::Approximation, {9, 3, 1});

        ScalingLayer* scaling_layer_pointer = neural_network.get_scaling_layer_pointer();

        scaling_layer_pointer->set_descriptives(inputs_descriptives);

        scaling_layer_pointer->set_scaling_methods(ScalingLayer::MinimumMaximum);

        // Training strategy

        TrainingStrategy training_strategy(&neural_network, &data_set);

        training_strategy.set_loss_method(TrainingStrategy::WEIGHTED_SQUARED_ERROR);

        training_strategy.get_loss_index_pointer()->set_regularization_method(LossIndex::RegularizationMethod::L2);
        training_strategy.get_loss_index_pointer()->set_regularization_weight(0.001);

        QuasiNewtonMethod* quasi_Newton_method_pointer = training_strategy.get_quasi_Newton_method_pointer();

        quasi_Newton_method_pointer->set_loss_goal(1.0e-3);

        quasi_Newton_method_pointer->set_display(true);

        training_strategy.set_display(true);

        training_strategy.perform_training();

        // Model selection

        ModelSelection model_selection(&training_strategy);

        model_selection.perform_neurons_selection();

        // Testing analysis

        data_set.unscale_inputs_minimum_maximum(inputs_descriptives);

        TestingAnalysis testing_analysis(&neural_network, &data_set);

        Matrix<size_t> confusion = testing_analysis.calculate_confusion();

        hpx::cout << "Confusion: " << hpx::endl;
        hpx::cout << confusion << hpx::endl;

        Vector<double> binary_classification_tests = testing_analysis.calculate_binary_classification_tests();

	hpx::future<void> f = hpx::async(print,binary_classification_tests);

        /*cout << "Binary classification tests: " << endl;
        cout << "Classification accuracy         : " << binary_classification_tests[0] << endl;
        cout << "Error rate                      : " << binary_classification_tests[1] << endl;
        cout << "Sensitivity                     : " << binary_classification_tests[2] << endl;
        cout << "Specificity                     : " << binary_classification_tests[3] << endl;
        cout << "Precision                       : " << binary_classification_tests[4] << endl;
        cout << "Positive likelihood             : " << binary_classification_tests[5] << endl;
        cout << "Negative likelihood             : " << binary_classification_tests[6] << endl;
        cout << "F1 score                        : " << binary_classification_tests[7] << endl;
        cout << "False positive rate             : " << binary_classification_tests[8] << endl;
        cout << "False discovery rate            : " << binary_classification_tests[9] << endl;
        cout << "False negative rate             : " << binary_classification_tests[10] << endl;
        cout << "Negative predictive value       : " << binary_classification_tests[11] << endl;
        cout << "Matthews correlation coefficient: " << binary_classification_tests[12] << endl;
        cout << "Informedness                    : " << binary_classification_tests[13] << endl;
        cout << "Markedness                      : " << binary_classification_tests[14] << endl;*/

        hpx::cout << "End\n" << hpx::endl;

        return 0;
    }
    catch(exception& e)
    {
        cerr << e.what() << endl;

        return 1;
    }
}



