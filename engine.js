



class Token
{
    constructor(content)
    {
        this._content = content;
    }
    _content;
    get content()    { return _content; } 
    get contentTeX() { return content; }
}

    class TrashToken extends Token
    {
        get content()    { return this._content; } 
        get contentTeX() { return this._content; }
    }


    class BracketOpen extends Token
    {
        get content()    { return "("; } 
        get contentTeX() { return "\\left("; }
    }

    class BracketClose extends Token
    {
        get content()    { return ")"; } 
        get contentTeX() { return "\\right)"; }
    }


    class Coma extends Token
    {
        get content()    { return ","; } 
        get contentTeX() { return ","; }
    }





    class OperatorToken extends Token
    {
        get priority()   { return 0; }
    }

        class UnaryOperatorToken extends OperatorToken
        {
            get priority()   { return 4; }
            get inputFrom()  { return 0; }
        }

            class Function extends UnaryOperatorToken
            {
                constructor(functionName)
                {
                    this._content = functionName;
                }
                get content()    { return this._content; } 
                get contentTeX() { return "\\text{" + this._content + "}"; }
                get inputFrom()  { return 1; }
            }

            class Factorial extends BinaryOperatorToken
            {
                get content()    { return "!"; }
                get contentTeX() { return "!"; }
                get inputFrom()  { return -1; }
            }

        class BinaryOperatorToken extends OperatorToken
        {

        }

            class Addition extends BinaryOperatorToken
            {
                get content()    { return "+"; } 
                get contentTeX() { return "+"; }
                get priority()   { return 1; }
            }

            class Substraction extends BinaryOperatorToken
            {
                get content()    { return "-"; } 
                get contentTeX() { return "-"; }
                get priority()   { return 1; }
            }

            class Multiplication extends BinaryOperatorToken
            {
                get content()    { return "*"; } 
                get contentTeX() { return "\\cdot"; }
                get priority()   { return 2; }
            }

            class Division extends BinaryOperatorToken
            {
                get content()    { return "/"; } 
                get contentTeX() { return "\\over"; }
                get priority()   { return 2; }
            }

            class Modulus extends BinaryOperatorToken
            {
                get content()    { return "%"; } 
                get contentTeX() { return "\\%"; }
                get priority()   { return 2; }
            }

            class Power extends BinaryOperatorToken
            {
                get content()    { return "^"; } 
                get contentTeX() { return "^"; }
                get priority()   { return 3; }
            }

    class OperandToken extends Token
    {

    }

        class Constant extends OperandToken
        {
            constructor(content)
            {
                this._content = content;
            }
            get content()    { return this._content; } 
            get contentTeX() { return this._content; }
        }

        class Variable extends OperandToken
        {
            constructor(name)
            {
                this._content = name;
            }
            get content()    { return this._content; } 
            get contentTeX() { return this._content; }
        }













