# Compiler 

## How to use 

1)  Cloning repository  <pre>git clone https://github.com/kubasikisikola2/compiler
 cd compiler/ </pre>
2)  Building the compiler  <pre>make</pre>
3)  Running the compiler <pre>./parser -i inputFile.txt [--print-ast --yydebug]</pre>
- --print-ast prints out the input program AST
- -yydebug prints additional info when parsing the input
- to run all the files in the tests/ directory use: <pre>make test</pre>
## Language
### File structure
- structure definitions
- main function
- function definitions
### Variable types
- I (Integer)
- D (Decimal number)
- B (Boolean value: T or F)
### Variable declaration
\<dataType\> \<identifier\> \<optionalAssignment\>  
#### Examples:  
- I num = 10;
- B boolVal;
### Operators
- Math: \+ \- / * ^ %
- Comparison: < > <= >= ==
- Logic: && ||
### If statements
- ? = if keyword
- ?e = else if statement
- e = else statement
#### Example:
<pre>?(num1 > num2){ 
    p(num1); 
}?e(num1 == 10){ 
    p(num2);
}e{ 
    p(10); 
}</pre>
### Loops
- while / for loop depending on the expression in brackets after keyword
- keyword = l
- l(\<comparation\> / \<rangeSpecification\>) {\<commands\>}
- range specification should look like this \<startValue\>..\<idxVariableName\>=\<endValue\>
#### Example while loop:
<pre>l(num < 10){
    num = num + 1;
}</pre>
#### Example for loop:
<pre>l(0..idx=10){
    p(idx);
}</pre>
### Structures
- structure definitions should be at the top of input file
- structure definition: s \<structureTypeIdentifier\> {\<memberDeclarations\>}
 #### Example:
 <pre>s Rectangle{
   D x1;
   D y1;
   D x2;
   D y2;
  }</pre>
- structure declaration: s \<structureTypeIdentifier\> \<identifier\>;
- accessing member" \<identifier\>.\<member\>
 #### Example:
 <pre>s Rectangle rect1;
 rect1.x1 = 10.32;
 rect1.y1 = 45.78;
 p(rect1.y1);</pre>
 ### Functions
- function declaration should be at the bottom of the input file
- function definition: \<returnType\> f \<functionIdentifier\> \{commands\}
- value is returned with keyword r
- function is called by: \<functionIdentifier\>(\<functionParams\>);
#### Example
<pre>I f sum(I num1, I num2){
     r num1 + num2;
}</pre>
### Print function
- function call: = p(\<param\>)
- param can be: literal, variable, math epression, logic expression, function call
  
