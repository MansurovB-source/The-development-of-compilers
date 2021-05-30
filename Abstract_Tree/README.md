## Grammar

```
<Программа> ::= <Объявление переменных> <Описание вычислений>
<Описание вычислений> ::= Begin <Список операторов> End
<Объявление переменных> ::= Var <Список переменных> ;
<Список переменных> ::= <Идент> | <Идент> , <Список переменных>
<Список операторов> ::= <Оператор> | <Оператор> <Список операторов>
<Оператор> ::= <Присваивание> | <Сложный оператор> | <Составной оператор>
<Составной оператор> ::= Begin <Спосок операторов> End
<Присваивание> ::= <Идент> = <Выражение> ;
<Выражение> ::= <Ун.оп.> <Подвыражение> | <Подвыражение>
<Подвыражение> ::= ( <Выражение> ) | <Операнд> | <Подвыражение> <Бин.оп.> <Подвыражение>
<Ун.оп.> ::= "-" | not
<Бин.оп.> ::= "-" | "+" | "*" | "/" | "<" | ">" | "=="
<Операнд> ::= <Идент> | <Const>
<Сложный оператор> ::= if (<Выражение>) <Оператор>
    | if (<Выражение>) <Оператор> else <Оператор>
    | <Оператор цикла>
<Оператор цикла> ::= Repeat <Список операторов> Until <Выражение>
<Составной оператор> ::= { <Список операторов> }
<Идент> ::= <Буква> <Идент> | <Буква>
<Const> ::= <Цифра> <Const> | <Цифра>
```

## Example

```
Var a, b, c, d;
Begin
	a = 8;
	b = 9;
	c = 5;
	d = 4;

	if (((a + b) + 9) > 0) 
	    c = (-a) + 2;

	else
	    Begin	
	        d = c / 3;
	        b = 2 + d * 2;
	    End

	d = c + a;
	a = 7 + c;

	Repeat
		Begin
		    b = c + 4;
		    a = b + 5;
		End
	Until (a == 6)
End
```
 ## Generated Tree
 ```
 +----[Programm_Body](NULL)
                                                  +----[Identifier](6)
                                             +----[Expression](==)
                                                  +----[Identifier](a)
                                        +----[Loop](NULL)
                                             +----[Operators](NULL)
                                                                 +----[Identifier](5)
                                                            +----[Expression](+)
                                                                 +----[Identifier](b)
                                                       +----[Operator](=)
                                                            +----[Identifier](a)
                                                  +----[Operators](NULL)
                                                                 +----[Identifier](4)
                                                            +----[Expression](+)
                                                                 +----[Identifier](c)
                                                       +----[Operator](=)
                                                            +----[Identifier](b)
                                   +----[Operators](NULL)
                                                  +----[Identifier](c)
                                             +----[Expression](+)
                                                  +----[Identifier](7)
                                        +----[Operator](=)
                                             +----[Identifier](a)
                              +----[Operators](NULL)
                                             +----[Identifier](a)
                                        +----[Expression](+)
                                             +----[Identifier](c)
                                   +----[Operator](=)
                                        +----[Identifier](d)
                         +----[Operators](NULL)
                                        +----[Operators](NULL)
                                                                 +----[Identifier](2)
                                                            +----[Expression](*)
                                                                 +----[Identifier](d)
                                                       +----[Expression](+)
                                                            +----[Identifier](2)
                                                  +----[Operator](=)
                                                       +----[Identifier](b)
                                             +----[Operators](NULL)
                                                            +----[Identifier](3)
                                                       +----[Expression](/)
                                                            +----[Identifier](c)
                                                  +----[Operator](=)
                                                       +----[Identifier](d)
                                   +----[Condition_Expression](NULL)
                                                  +----[Identifier](2)
                                             +----[Expression](+)
                                                       +----[Identifier](a)
                                                  +----[Expression](MINUS)
                                        +----[Operator](=)
                                             +----[Identifier](c)
                              +----[If_Else](NULL)
                                        +----[Identifier](0)
                                   +----[Expression](>)
                                             +----[Identifier](9)
                                        +----[Expression](+)
                                                  +----[Identifier](b)
                                             +----[Expression](+)
                                                  +----[Identifier](a)
                    +----[Operators](NULL)
                              +----[Identifier](4)
                         +----[Operator](=)
                              +----[Identifier](d)
               +----[Operators](NULL)
                         +----[Identifier](5)
                    +----[Operator](=)
                         +----[Identifier](c)
          +----[Operators](NULL)
                    +----[Identifier](9)
               +----[Operator](=)
                    +----[Identifier](b)
     +----[Operators](NULL)
               +----[Identifier](8)
          +----[Operator](=)
               +----[Identifier](a)
[Root](NULL)
                    +----[Variables](d)
               +----[Variables](c)
          +----[Variables](b)
     +----[Variables](a)
+----[Variables_Declaration](NULL)
```

## Generated Assembly
```
a = 8
b = 9
c = 5
d = 4
tmp_2 = a + b
tmp_1 = tmp_2 + 9
tmp_0 = tmp_1 > 0
cmp_true tmp_0 label_0
tmp_4 = MINUS a
tmp_3 = tmp_4 + 2
c = tmp_3
goto label_1
label_0:
tmp_5 = c / 3
d = tmp_5
tmp_7 = d * 2
tmp_6 = 2 + tmp_7
b = tmp_6
label_1:
tmp_8 = c + a
d = tmp_8
tmp_9 = 7 + c
a = tmp_9
label_2:
tmp_11 = c + 4
b = tmp_11
tmp_12 = b + 5
a = tmp_12
tmp_10 = a == 6
cmp_true tmp_10 label_2
```