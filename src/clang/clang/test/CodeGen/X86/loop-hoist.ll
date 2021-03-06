; RUN: llvm-as < %s | \
; RUN:   llc -relocation-model=dynamic-no-pic -mtriple=i686-apple-darwin8.7.2 |\
; RUN:   grep L_Arr.non_lazy_ptr
; RUN: llvm-as < %s | \
; RUN:   llc -disable-post-RA-scheduler=true \
; RUN:       -relocation-model=dynamic-no-pic -mtriple=i686-apple-darwin8.7.2 |\
; RUN:   %prcontext L_Arr.non_lazy_ptr 1 | grep {4(%esp)}

@Arr = external global [0 x i32]		; <[0 x i32]*> [#uses=1]

define void @foo(i32 %N.in, i32 %x) {
entry:
	%N = bitcast i32 %N.in to i32		; <i32> [#uses=1]
	br label %cond_true

cond_true:		; preds = %cond_true, %entry
	%indvar = phi i32 [ %x, %entry ], [ %indvar.next, %cond_true ]		; <i32> [#uses=2]
	%i.0.0 = bitcast i32 %indvar to i32		; <i32> [#uses=2]
	%tmp = getelementptr [0 x i32]* @Arr, i32 0, i32 %i.0.0		; <i32*> [#uses=1]
	store i32 %i.0.0, i32* %tmp
	%indvar.next = add i32 %indvar, 1		; <i32> [#uses=2]
	%exitcond = icmp eq i32 %indvar.next, %N		; <i1> [#uses=1]
	br i1 %exitcond, label %return, label %cond_true

return:		; preds = %cond_true
	ret void
}
