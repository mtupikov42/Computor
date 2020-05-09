add_library(ebst STATIC
	EBST.h
	EBST.cpp
	NodeHelpers.cpp
	NodeRules.cpp
	NodeOutput.cpp
	NodeReduce.cpp
	NodeBalancing.cpp
	NodeSimplifiers.cpp
	NodeSolver.cpp

	ExpressionException.h
	ExpressionException.cpp

	ExpressionNode.h #remove
	ExpressionNode.cpp #remove

	AbstractExpressionNode.h
	AbstractExpressionNode.cpp
	NumberNode.h
	NumberNode.cpp
	ImaginaryNumberNode.h
	ImaginaryNumberNode.cpp
	OperatorNode.h
	OperatorNode.cpp
	UnknownNode.h
	UnknownNode.cpp
)

target_link_libraries(ebst PUBLIC abst)

target_include_directories(ebst INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
