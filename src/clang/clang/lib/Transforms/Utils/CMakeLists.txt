add_llvm_library(LLVMTransformUtils
  AddrModeMatcher.cpp
  BasicBlockUtils.cpp
  BasicInliner.cpp
  BreakCriticalEdges.cpp
  CloneFunction.cpp
  CloneLoop.cpp
  CloneModule.cpp
  CloneTrace.cpp
  CodeExtractor.cpp
  DemoteRegToStack.cpp
  InlineCost.cpp
  InlineFunction.cpp
  LCSSA.cpp
  Local.cpp
  LoopSimplify.cpp
  LowerAllocations.cpp
  LowerInvoke.cpp
  LowerSwitch.cpp
  Mem2Reg.cpp
  PromoteMemoryToRegister.cpp
  SimplifyCFG.cpp
  UnifyFunctionExitNodes.cpp
  UnrollLoop.cpp
  ValueMapper.cpp
  InstructionNamer.cpp
  )
