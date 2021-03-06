# iVM
A Stack-based VM

# Demo
**test_vm.lisp**
```lisp
(* (+ (+ (+ 1 2) 3) 4) -5)
```

**test_chunk.vm**

```assemble 
== test_chunk.vm ==
0000    1 OP_CONSTANT         0 '1'
0002    1 OP_CONSTANT         1 '2'
0004    1 OP_ADD
0005    1 OP_CONSTANT         2 '3'
0007    1 OP_ADD
0008    1 OP_CONSTANT         3 '4'
0010    1 OP_ADD
0011    1 OP_CONSTANT         4 '-5'
0013    1 OP_MULTIPLY
0014  666 OP_RETURN
0000    1 OP_CONSTANT         0 '1'
```

**Result Log**
```
0000    1 OP_CONSTANT         0 '1'
offset:0 
stack 
0002    1 OP_CONSTANT         1 '2'
offset:2 
stack [ 1 ]
...
stack [ 10 ][ -5 ]
0014  666 OP_RETURN
offset:14 
stack [ -50 ]
-50
```
