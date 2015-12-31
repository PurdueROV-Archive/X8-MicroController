
#ifndef CLASS_NAME_IN_CAPS
#define CLASS_NAME_IN_CAPS

/* puts all of the #includes and # defines that you need here */


class classname {


public:
	/* Put function declarations that the user will
	   want to use from the class here */


/*constructor */
classname(put_input_parameters_here);

int otherFunction(other_input_parameters);


private: 
	/* Put function delcarations or functions that 
	the class will use but the user will not have 
	access to */

	int temp_variable;  //this variable can only be accessed by the specific object

};

#endif