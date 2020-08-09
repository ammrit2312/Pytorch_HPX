# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8.10)
project(opennn)
find_package(HPX REQUIRED)
#target_include_directories(${PROJECT_NAME} PUBLIC ${HPX_INCLUDE_DIR})

# Project's name

add_hpx_library(opennn
	SOURCES adaptive_moment_estimation.cpp
		bounding_layer.cpp
		conjugate_gradient.cpp
		convolutional_layer.cpp 	
		correlations.cpp
		cross_entropy_error.cpp
		data_set.cpp
		functions.cpp
		genetic_algorithm.cpp
		gradient_descent.cpp
		growing_inputs.cpp
		incremental_neurons.cpp
		inputs_selection.cpp
		k_means.cpp
		layer.cpp
		learning_rate_algorithm.cpp
		levenberg_marquardt_algorithm.cpp
		long_short_term_memory_layer.cpp
		loss_index.cpp
		mean_squared_error.cpp
		metrics.cpp
		minkowski_error.cpp
		model_selection.cpp
		neural_network.cpp
		neurons_selection.cpp
		normalized_squared_error.cpp
		numerical_differentiation.cpp
		opennn_strings.cpp
		optimization_algorithm.cpp
		perceptron_layer.cpp
		pooling_layer.cpp
		principal_components_layer.cpp
		probabilistic_layer.cpp
		pruning_inputs.cpp
		quasi_newton_method.cpp
		recurrent_layer.cpp
		response_optimization.cpp
		scaling_layer.cpp
		statistics.cpp
		stochastic_gradient_descent.cpp
		sum_squared_error.cpp
		testing_analysis.cpp
		tinyxml2.cpp
		training_strategy.cpp
		transformations.cpp
		unit_testing.cpp
		unscaling_layer.cpp
		weighted_squared_error.cpp





	HEADERS adaptive_moment_estimation.h
		bounding_layer.h
		conjugate_gradient.h
		convolutional_layer.h 	
		correlations.h
		cross_entropy_error.h
		data_set.h
		functions.h
		genetic_algorithm.h
		gradient_descent.h
		growing_inputs.h
		incremental_neurons.h
		inputs_selection.h
		k_means.h
		layer.h
		learning_rate_algorithm.h
		levenberg_marquardt_algorithm.h
		long_short_term_memory_layer.h
		loss_index.h
		mean_squared_error.h
		metrics.h
		minkowski_error.h
		model_selection.h
		neural_network.h
		neurons_selection.h
		normalized_squared_error.h
		numerical_differentiation.h
		opennn_strings.h
		optimization_algorithm.h
		perceptron_layer.h
		pooling_layer.h
		principal_components_layer.h
		probabilistic_layer.h
		pruning_inputs.h
		quasi_newton_method.h
		recurrent_layer.h
		response_optimization.h
		scaling_layer.h
		statistics.h
		stochastic_gradient_descent.h
		sum_squared_error.h
		testing_analysis.h
		tinyxml2.h
		training_strategy.h
		transformations.h
		unit_testing.h
		unscaling_layer.h
		weighted_squared_error.h
		

	COMPONENT_DEPENDENCIES iostreams)

#target_link_libraries(${PROJECT_NAME} PUBLIC "${HPX_LIBRARIES}")
