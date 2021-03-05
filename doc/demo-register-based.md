

# Concepts

## Why Register-based VM [0]

Register-based code avoids several “push” and “pop” instructions that stack- based code 

needs to move values around the stack. Those instructions are par- ticularly expensive in 

Lua, because they involve the copy of a tagged value.

For ten years (since 1993, when Lua was first released), Lua used a stack- based virtual 

machine, in various incarnations. Since 2003, with the release of Lua 5.0, Lua uses a 

register-based virtual machine. This register-based machine also uses a stack, for 

allocating activation records, wherein the registers live. When Lua enters a function, it 

preallocates from the stack an activation record large enough to hold all the function 

registers. All local variables are allocated in registers. As a consequence, access to 

local variables is specially efficient.


## Problems of Register-based VM [0]

There are two problems usually associated with register-based machines: **code size and** 

**decoding overhead**. An instruction in a register machine needs to specify its operands,
 
and so it is typically larger than a corresponding instruction in a stack machine. (For
 
instance, the size of an instruction in Lua’s virtual machine is four bytes, while the 

size of an instruction in several typical stack machines, including the ones previously 

used by Lua, is one or two bytes.) On the other hand, register machines generate less 

opcodes than stack machines, so the total code size is not much larger.

Most instructions in a stack machine have implicit operands. The corresponding 

instructions in a register machine must decode their operands from the in- struction. 

Such decoding adds overhead to the interpreter. There are several factors that ameliorate 

this overhead.

First, stack machines also spend some time manipulating implicit operands (e.g., to 

increment or decrement the stack top). 

Second, because in a register machine all operands are inside the instruc- tion and the 

instruction is a machine word, the operand decoding involves only cheap operations, such 

as logical operations. 

Moreover, instructions in stack machines frequently need multi-byte operands. Due to 

alignment, the in- terpreter cannot fetch such operands at once. On a register machine, 

because the operands are inside the instruction, the interpreter does not have to fetch 

them independently.


## Instruction Set [0]

There are **35 instructions** in Lua’s virtual machine.

Most instructions were chosen to correspond directly to language constructs: arithmetic, 

table creation and indexing, function and method calls, setting variables and getting 

values.

There is also a set of conventional jump instructions to implement control structures. 

### Mathematics Model

**R(X)** means the **Xth** register. **K(X)** means the **Xth** constant. **RK(X)** means 

either **R(X)** or **K(X-k)**, depending on the value of **X** — it is **R(X)** for values 

of **X** smaller than **k** (a build parameter, typically 250). **G[X]** means the field 

**X** in the table of globals. **U[X]** means the **Xth** upvalue.

MOVE		A	B			R(A) := R(B)

LOADK		A	Bx			R(A) := K(Bx)

LOADBOOL	A	B	C		R(A) := (Bool)B; if (C) PC++

LOADNIL		A	B			R(A) := ... R(B) := nil

GETUPVAL	A	B			R(A) := U[B]

...


### Performance

Registers are kept in the run-time stack, which is essentially an array. Thus, access to 

registers is fast. Constants and upvalues are stored in arrays and so access to them is 

also fast.

The table of globals is an ordinary Lua table. It is accessed via hashing but with good 

performance, because it is indexed only with strings (corresponding to variable names), 

and strings pre-compute their hash values.



# Reference

[0] [Section 7, The Virtual Machine (Lua5.0)](https://www.lua.org/doc/jucs05.pdf)


