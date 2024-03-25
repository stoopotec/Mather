



class Token
{
    get TokenName()  { return "Token"; }
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
        get TokenName()  { return "TrashToken"; }
        get content()    { return this._content; } 
        get contentTeX() { return this._content; }
    }


    class BracketOpen extends Token
    {
        get TokenName()  { return "BracketOpen"; }
        get content()    { return "("; } 
        get contentTeX() { return "\\left("; }
    }

    class BracketClose extends Token
    {
        get TokenName()  { return "BracketClose"; }
        get content()    { return ")"; } 
        get contentTeX() { return "\\right)"; }
    }


    class Coma extends Token
    {
        get TokenName()  { return "Coma"; }
        get content()    { return ","; } 
        get contentTeX() { return ","; }
    }


    class Equal extends Token
    {
        get TokenName()  { return "Equal"; }
        get content()    { return "="; } 
        get contentTeX() { return "="; }
    }





    class OperatorToken extends Token
    {
        get TokenName()  { return "OperatorToken"; }
        get priority()   { return 0; }
    }

        class UnaryOperatorToken extends OperatorToken
        {
            get TokenName()  { return "UnaryOperatorToken"; }
            get priority()   { return 4; }
            get inputFrom()  { return 0; }
        }

            class Function extends UnaryOperatorToken
            {
                get TokenName()  { return "Function"; }
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
                get TokenName()  { return "Factorial"; }
                get content()    { return "!"; }
                get contentTeX() { return "!"; }
                get inputFrom()  { return -1; }
            }

        class BinaryOperatorToken extends OperatorToken
        {
            get TokenName()  { return "BinaryOperatorToken"; }

        }

            class Addition extends BinaryOperatorToken
            {
                get TokenName()  { return "Addition"; }
                get content()    { return "+"; } 
                get contentTeX() { return "+"; }
                get priority()   { return 1; }
            }

            class Substraction extends BinaryOperatorToken
            {
                get TokenName()  { return "Substraction"; }
                get content()    { return "-"; } 
                get contentTeX() { return "-"; }
                get priority()   { return 1; }
            }

            class Multiplication extends BinaryOperatorToken
            {
                get TokenName()  { return "Multiplication"; }
                get content()    { return "*"; } 
                get contentTeX() { return "\\cdot"; }
                get priority()   { return 2; }
            }

            class Division extends BinaryOperatorToken
            {
                get TokenName()  { return "Division"; }
                get content()    { return "/"; } 
                get contentTeX() { return "\\over"; }
                get priority()   { return 2; }
            }

            class Modulus extends BinaryOperatorToken
            {
                get TokenName()  { return "Modulus"; }
                get content()    { return "%"; } 
                get contentTeX() { return "\\%"; }
                get priority()   { return 2; }
            }

            class Power extends BinaryOperatorToken
            {
                get TokenName()  { return "Power"; }
                get content()    { return "^"; } 
                get contentTeX() { return "^"; }
                get priority()   { return 3; }
            }

    class OperandToken extends Token
    {
        get TokenName()  { return "OperandToken"; }

    }

        class Constant extends OperandToken
        {
            get TokenName()  { return "Constant"; }
            get content()    { return this._content; } 
            get contentTeX() { return this._content; }
        }

        class Variable extends OperandToken
        {
            get TokenName()  { return "Variable"; }
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





class Expression
{
    constructor(tokens)
    {
        this.Tokens = tokens;
    }

    toString()
    {
        expressionToString(this);
    }
    Tokens;
}


const tokensToString = (tokens) => {
    let str = "";
    for (let i = 0; i < tokens.length; ++i)
    {
        str +=  tokens[i].content + " ";
    }
    return str;
}


const expressionToString = (expression) => {
    return tokensToString(expression.Tokens);
}




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


const constructPermutData = (expression) => {
    let equivalents = [[]];
    for (let i = 0; i < expression.Tokens.length; ++i) {
        if (expression.Tokens[i] instanceof Equal) {
            equivalents.push([]);
            continue;
        }
        equivalents[equivalents.length-1].push(expression.Tokens[i]);
    }


    let permutData = [];
    let obj = {};
    Object.assign(obj, expression);
    for (let i = 0; i < equivalents.length; ++i)
        for (let j = 0; j < equivalents.length; ++j) {
            if (i === j) continue;
            permutData.push({from: equivalents[i], to: equivalents[j]});
        }
    return permutData;
}


var allPermutations = [];
allPermutations.push(...constructPermutData(constructExpression("a+b=b+a")));
allPermutations.push(...constructPermutData(constructExpression("a+(b+c)=(a+b)+c")));
allPermutations.push(...constructPermutData(constructExpression("a+0=a")));
allPermutations.push(...constructPermutData(constructExpression("a+(-a)=0")));

allPermutations.push(...constructPermutData(constructExpression("a*b=b*a")));
allPermutations.push(...constructPermutData(constructExpression("a*(b*c)=(a*b)*c")));
allPermutations.push(...constructPermutData(constructExpression("1*a=a")));
allPermutations.push(...constructPermutData(constructExpression("a*(1/a)=1")));
allPermutations.push(...constructPermutData(constructExpression("a+(b*c)=a*b+a*c")));

// allPermutations.push(...constructPermutData(constructExpression("")));



const tokensSubarrayFromTo = (tokens, tokensSub) => {

    // console.log("\n\n");
    // console.log("tokens:", tokens);
    // console.log("tokens sub:", tokensSub);

    let fromToList = [];

    for (let i = 0; i < tokens.length; ++i) {
        let match = true;
        
        for (let j = i; j < tokensSub.length + i && j < tokens.length; ++j) {
            
            if (tokens[j].TokenName !== tokensSub[j - i].TokenName) {
                match = false;
                break;
            }
        
        }

        if (match) fromToList.push({ from: i, to: i + tokensSub.length });
    }

    return fromToList;
}

const getPermutations = (expression) => {

    let permuts = [];

    console.log("possible permutations:", allPermutations.length);

    for (let i = 0; i < allPermutations.length; ++i) {
        console.log("\n\n");

        console.log("permutation", i, "(" + tokensToString(allPermutations[i].from) + " =>  " + tokensToString(allPermutations[i].to) + ")");

        let fromToList = tokensSubarrayFromTo(expression.Tokens, allPermutations[i].from);
        console.log("from to list:", fromToList);

        for (let j = 0; j < fromToList.length; ++j ) {
            
            let obj = JSON.parse(JSON.stringify(expression));

            console.log("expression before:", expressionToString(obj));
            //obj.toString = Expression.toString;
            //console.log(expressionToString(obj));
            obj.Tokens.splice(fromToList[j].from, fromToList[j].to - fromToList[j].from);
            obj.Tokens.splice(fromToList[j].from, 0, ...allPermutations[i].to);
            //if (newExpr.length > 0)
            console.log("expression after:", expressionToString(obj));

            permuts.push(obj);
        }


    }

    return permuts;

}





