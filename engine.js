



class Token
{
    constructor(content)
    {
        this._content = content;
    }
    _content;
    get directContent() { return this._content; }
    get content()       { return _content; } 
    get contentTeX()    { return content; }
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


    class Equal extends Token
    {
        get content()    { return "="; } 
        get contentTeX() { return "="; }
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

            class Factorial extends UnaryOperatorToken
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
            get content()    { return this._content; } 
            get contentTeX() { return this._content; }
        }

        class Variable extends OperandToken
        {
            get content()    { return this._content; } 
            get contentTeX() { return this._content; }
        }




const regexes = 
[
//  {   token(content) { return new Token(content); },
//      regexp: RegExp(), 
//      contentRegexp: RegExp() 
//  },
    {   token(content) { return new TrashToken(content); },
        regexp:        /\s+/gm,
        contentRegexp: /\s+/gm
    },

    {   token(content) { return new BracketOpen(content); },
        regexp:        /\(/gm,
        contentRegexp: /\(/gm
    },
    {   token(content) { return new BracketClose(content); },
        regexp:        /\)/gm,
        contentRegexp: /\)/gm
    },
    {   token(content) { return new Equal(content); },
        regexp:        /=/gm,
        contentRegexp: /=/gm
    },


    {   token(content) { return new Factorial(content); },
        regexp:        /\!/gm,
        contentRegexp: /\!/gm
    },
    {   token(content) { return new Addition(content); },
        regexp:        /\+/gm,
        contentRegexp: /\+/gm
    },
    {   token(content) { return new Substraction(content); },
        regexp:        /\-/gm,
        contentRegexp: /\-/gm
    },
    {   token(content) { return new Multiplication(content); },
        regexp:        /\*/gm,
        contentRegexp: /\*/gm
    },
    {   token(content) { return new Division(content); },
        regexp:        /\//gm,
        contentRegexp: /\//gm
    },
    {   token(content) { return new Modulus(content); },
        regexp:        /\%/gm,
        contentRegexp: /\%/gm
    },
    {   token(content) { return new Power(content); },
        regexp:        /\^/gm,
        contentRegexp: /\^/gm
    },


    {   token(content) { return new Constant(content); },
        regexp:        /\.\d+|\d+\.\d+|\d+\.|\d+|\./gm, // https://regex101.com/r/XYvJOz/1
        contentRegexp: /\.\d+|\d+\.\d+|\d+\.|\d+|\./gm
    },
    {   token(content) { return new Variable(content); },
        regexp:        /[a-zA-Zа-яёА-ЯЁ]+/gm,
        contentRegexp: /[a-zA-Zа-яёА-ЯЁ]+/gm 
    },
];






const constructExpression = (text) =>
{
    let tokensMeta = [];
    // token, start, length

    for (let i = 0; i < regexes.length; ++i)
    {
        while ((m = regexes[i].regexp.exec(text)) !== null) {
            // This is necessary to avoid infinite loops with zero-width matches
            if (m.index === regexes[i].regexp.lastIndex) ++regexes[i].regexp.lastIndex;


            let tokenMeta = 
            {
                token: regexes[i].token(m[0]),
                start: m.index,
                length: m.length,
            };

            if (!(tokenMeta.token instanceof TrashToken))
                tokensMeta.push(tokenMeta);
        }
    }

    tokensMeta.sort((a, b) => a.start - b.start);

    let tokens = [];
    for (let i = 0; i < tokensMeta.length; ++i)
        tokens.push(tokensMeta[i].token);
    return new Expression(tokens);


}


class Expression
{
    constructor(tokens)
    {
        this.Tokens = tokens;
    }

    toString()
    {
        let str = "";
        for (let i = 0; i < this.Tokens.length; ++i)
        {
            str += this.Tokens[i].directContent;
        }
        return str;
    }
    Tokens;
}





