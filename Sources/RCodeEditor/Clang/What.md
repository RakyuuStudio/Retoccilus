### Folder Clang Code Explanation
Hey everyone, so if you want to see how the Static Analyzer for C-Family works, 
but you don't want to read the code?   

Here is your guidelines! Tell you how it works.
***
#### Chapter 1: AST Generator
You know the AST? No? That's OK! Let me told you about AST.  

AST (Abstract Syntax Tree), is a Tree Data Structure that shows you the
struct of the source file.  

And how to generate that? We integrated about Clang, so we could just use Clang
to generate the AST.

##### 1: Create an AST Consumer Class
