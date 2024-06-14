`LDAXR`指令是ARM架构（特别是包括AArch64执行状态的ARMv8）中的一个原子加载独占指令。它是ARM独占访问的一部分，用于实现像互斥锁和信号量这样的同步原语。

## `LDAXR`指令实现和工作方式的分解：
Page 397  
ISA_A64_xml_A_profile-2024-03

1. **地址计算**：指令计算用来加载数据的内存地址。这个地址是从基址寄存器(`<Xn|SP>`)派生的，可以是通用寄存器或堆栈指针。如果基址寄存器是`SP`且值为`31`，则检查堆栈指针的对齐。

2. **独占访问标记**：处理器标记正在访问的物理地址为独占访问。这个标记用于确保随后对同一地址的存储独占指令（`STXR`）可以确定是否有其他处理器或观察者介入访问了标记位置，这将使独占状态无效。

3. **内存访问**：指令执行从计算出的内存地址进行的原子加载操作。加载的数据大小可以是32位字或64位双字，由指令编码中的`size`字段指示。

4. **数据传输**：加载的数据被写入目的寄存器(`<Wt>`用于32位或`<Xt>`用于64位)。如果加载的数据小于寄存器大小，则进行零扩展以适应寄存器。

5. **内存排序语义**：`LDAXR`指令还包括获取语义。这意味着程序顺序中跟随`LDAXR`的任何内存访问不能在它之前移动。这确保了加载操作在后续内存操作执行之前看起来已经完成。

6. **独占监视器设置**：ARM架构维护独占监视器，这些监视器跟踪地址的独占状态。`LDAXR`指令通知独占监视器记录原子读取操作。只有当所有读取都来自同一对齐的物理地址时，监视器才会设置独占状态，允许原子性。

7. **操作信息**：如果`PSTATE.DIT`（数据独立定时）状态设置为`1`，则`LDAXR`指令的定时将对加载的数据值不敏感。这可以用来通过使操作的定时独立于数据来减轻某些类型的侧信道攻击。

`LDAXR`指令通常与`STXR`（存储独占寄存器）指令一起使用，以创建原子读-修改-写序列。例如，在一个简单的自旋锁实现中，一个线程可能使用`LDAXR`来加载锁的值并检查它是否是空闲的，然后使用`STXR`尝试存储一个新值来获取锁。如果`STXR`失败了，因为独占状态丢失了（意味着另一个线程访问了内存），线程将重试该序列。

总之，`LDAXR`是在ARM架构上的多线程环境中实现同步机制的关键指令。它确保了关键代码段的原子性和适当的内存排序。


## Why Check register31
在ARM架构中，寄存器编号31通常代表堆栈指针（SP），特别是在AArch64执行状态中。当指令中使用寄存器编号31时，通常表示该指令与堆栈指针相关联。在某些情况下，需要确保堆栈指针是正确对齐的，这是因为堆栈的对齐对于系统的稳定运行和性能至关重要。

对齐检查是为了确保堆栈指针满足特定的对齐要求，这通常是16字节对齐，这样可以保证访问堆栈上的数据时具有最佳的性能，并且遵守操作系统和硬件的要求。如果堆栈指针未正确对齐，可能会导致性能下降，或者在更糟糕的情况下，导致硬件异常。

在ARM架构中，对齐检查通常是自动进行的，当使用寄存器编号31作为基址寄存器时，硬件或者指令集架构会要求自动检查堆栈指针的对齐。如果检查失败，即堆栈指针没有正确对齐，程序可能会触发一个对齐异常，这通常会导致异常处理程序被调用来处理这个问题。

因此，当基址寄存器编号为31时，检查堆栈指针的对齐是为了保证系统的正确运行和性能优化，并且避免因为对齐问题导致的异常或错误。

## ExclusiveMonitors  
在ARM架构中，`AArch64.SetExclusiveMonitors`函数用于设置当前处理器元素（PE）的独占监视器，以记录从给定虚拟地址`address`开始、大小为`size`字节的虚拟地址区域相关联的地址。这是实现独占访问操作的一部分，例如`LDAXR`和`STLXR`指令。

以下是`AArch64.SetExclusiveMonitors`函数的伪代码实现及其中文解释：

```pseudocode
// AArch64.SetExclusiveMonitors()
// 设置当前PE的独占监视器，以记录与从address开始、大小为size字节的虚拟地址区域相关联的地址。
AArch64.SetExclusiveMonitors(bits(64) address, integer size)
    boolean acqrel = FALSE;
    boolean tagchecked = FALSE;
    AccessDescriptor accdesc = CreateAccDescExLDST(MemOp_LOAD, acqrel, tagchecked);
    boolean aligned = IsAligned(address, size);
    if !aligned && AArch64.UnalignedAccessFaults(accdesc, address, size) then
        AArch64.Abort(address, AlignmentFault(accdesc));
    memaddrdesc = AArch64.TranslateAddress(address, accdesc, aligned, size);
    // 检查中止或调试异常
    if IsFault(memaddrdesc) then
        return;
    if memaddrdesc.memattrs.shareability != Shareability_NSH then
        MarkExclusiveGlobal(memaddrdesc.paddress, ProcessorID(), size);
        MarkExclusiveLocal(memaddrdesc.paddress, ProcessorID(), size);
    AArch64.MarkExclusiveVA(address, ProcessorID(), size);
```

解释：

1. `acqrel`和`tagchecked`变量初始化为`FALSE`，表示不需要获取释放语义和标签检查。

2. 调用`CreateAccDescExLDST`函数创建一个访问描述符`accdesc`，指定内存操作类型为加载（MemOp_LOAD），并且不需要获取释放语义和标签检查。

3. `IsAligned`函数检查给定的地址是否按照`size`大小对齐。

4. 如果地址未对齐且未对齐访问会导致故障，则调用`AArch64.Abort`函数中止执行，并报告对齐故障。

5. `TranslateAddress`函数将虚拟地址`address`转换为物理地址描述符`memaddrdesc`，同时考虑对齐和大小。

6. 如果地址转换结果是故障，则直接返回，不再继续执行。

7. 如果转换后的内存属性`memattrs.shareability`不等于非共享（Shareability_NSH），则进行全局和本地独占标记：

   - `MarkExclusiveGlobal`函数在全局独占监视器中标记物理地址`memaddrdesc.paddress`。
   
   - `MarkExclusiveLocal`函数在本地独占监视器中标记物理地址`memaddrdesc.paddress`。

8. `AArch64.MarkExclusiveVA`函数标记虚拟地址`address`，以便后续的`STLXR`指令可以检查是否有其他处理器介入了这个地址的访问。

通过上述步骤，`AArch64.SetExclusiveMonitors`函数为实现独占访问操作设置了必要的监视器状态。这些监视器用于跟踪对特定内存地址的独占访问尝试，确保在多处理器环境中同步操作的正确性。

## CreateAccDescExLDST Details

`CreateAccDescExLDST`函数不是实际的ARM指令，而是在ARM架构的伪代码描述中使用的概念性函数，用于表示为独占加载/存储操作创建访问描述符。这种伪代码用于ARM架构参考手册中，以正式的方式描述某些指令的行为。

概念上，`CreateAccDescExLDST`函数会创建一个`AccessDescriptor`对象，该对象包含有关如何执行内存访问的信息，特别是对于像`LDAXR`和`STXR`这样的独占加载和存储操作。访问描述符通常包括有关内存操作类型、访问是否需要获取-释放语义、以及是否需要执行额外检查（如地址标签检查）的详细信息。

以下是这种函数通常涉及的参数的详细信息：

- **MemOp**：指定内存操作的类型。这可以是加载（`MemOp_LOAD`）或存储（`MemOp_STORE`）操作。对于独占操作，此参数将指示访问是用于同步的原子序列的一部分。

- **AcquireRelease**：一个布尔值，指示访问是否具有获取-释放语义。获取语义确保后续内存访问不会在当前访问之前移动，而释放语义确保先前内存访问不会在当前访问之后移动。这对于在多线程环境中维护内存顺序非常重要。

- **TagChecked**：一个布尔值，指示是否应执行地址标签检查。在某些系统中使用地址标签为内存访问提供额外的检查，这对于安全或确保在具有某些类型内存的系统上正确操作很重要。

在实际的ARM汇编中，这些细节被编码在指令本身内部，并由处理器的内存管理单元（MMU）和其他控制逻辑处理。访问描述符将是这些信息的概念性表示，处理器用它来确定如何执行内存访问。

在实践中，当用汇编编程或使用更高级的构造时，你不会直接创建访问描述符；相反，你使用具有这些行为的特定指令，这些行为通过它们的操作隐含地定义。像`CreateAccDescExLDST`这样的伪代码函数被ARM用在他们的文档中，以详细和精确的方式解释这些指令的行为。

## Hardware implementation of ExclusiveMonitors
截至我知识更新的时间点（2023年），ARM架构的硬件实现细节，包括独占监视器（Exclusive Monitors）的具体设计，通常不会被详细公开。这些信息属于处理器设计的专有部分，通常只有处理器设计者和制造商才能访问。

然而，我们可以根据ARM架构的公开文档和一些通用的概念来了解独占监视器的基本工作原理：

1. **独占监视器的作用**：独占监视器用于支持同步指令，如LDAXR（加载独占）和STLXR（存储独占）。这些指令用于构建锁和其他同步机制。

2. **物理实现**：独占监视器可能由一组特殊的寄存器实现，这些寄存器记录了由独占加载指令访问的内存地址和相关的状态信息。

3. **监视器的范围**：每个处理器核心可能有自己的本地独占监视器，而在多核处理器中，还可能有全局独占监视器来跟踪跨核心的独占状态。

4. **缓存一致性**：在多核处理器中，独占监视器与缓存一致性协议（如MESI或MOESI）紧密集成，以确保在核心之间维护独占状态的正确性。

5. **独占状态的清除**：当其他处理器访问了被标记为独占的内存地址，或者当执行了影响内存地址的缓存维护操作时，独占状态会被清除。

6. **性能优化**：为了提高性能，独占监视器的设计会尽量减少对处理器性能的影响，例如，通过限制监视器状态的数量来减少资源使用。

7. **处理器间通信**：在多核系统中，处理器间通信机制（如中断或信号）可能被用来协调独占状态的清除和设置。

由于ARM架构允许不同的硬件制造商在遵循ARM指令集架构（ISA）的基础上进行自定义实现，不同的ARM处理器可能会有不同的独占监视器实现方式。因此，如果你需要关于特定ARM处理器最新的独占监视器的硬件实现细节，最好的做法是参考该处理器制造商发布的技术文档和数据手册。

# registers -> ExclusiveMonitor -> AccDescExLDST -> LDAXR/STLXR -> atomic -> spin-lock