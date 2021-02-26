

The issue is to create a complete VM but not for a good language ? => YES

So to create a complete VM, we still need a AST tree established.

1. Now, we scan the AST and transform it into a LIST.

2. Then we scan the LIST and transform it into ASSEMBLE codes.

3. Then we scan the ASSEMBLE codes and decode them into the stack.

4. Then we execute the instructions from the stack. If the instructions is found located
   at a LIB then we need to load them(do step-3 again)
   
5. Looping.


The goal here is to do more practices about VM so as to understand how to create a baby
language using a VM.


1. Run the demo in Xcode.

2. Do the steps as mentioned above to implement demo1.

3. Implement professional auto tests.
