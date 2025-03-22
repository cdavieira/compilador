; Dumping string table

; Dumping ftable
declare i32 @printf(i8*, ...)
declare i32 @scanf(i8*, ...)

; Writing function
define i32 @main(){
	; Loading i32 literal
	%1 = alloca i32, align 4
	store i32 0, ptr %1, align 4
	%2 = load i32, ptr %1, align 4

	ret i32 %2

}

; Writing function
define float @func4(){
}

; Writing function
define void @func3(float, i32){
}

; Writing function
define void @func2(){
}

; Writing function
define i32 @func1(){
}

