



const produceTransitions = () =>
{

  console.log("hello");

  let equation = document.getElementById("equation").value;
    
    console.log(equation);

    fetch("", {
      method: "CALC",
      headers: {
        "Content-Type": "text/plain",
      },
      body: equation
    })
    .then(response => response.body)
    .then(data => {
      console.log("Ответ от сервера:", data);
    })
    .catch(error => {
      console.error("Ошибка:", error);
    });





  let expr = constructExpression(document.getElementById("equation").value); 
  console.log(expr, expr.toString());
  let exprs = getPermutations(expr);
  console.log(exprs[0], exprs[0].toString());

  
  
  let container = document.getElementsByClassName("transitions-container")[0];
  let templateHTML = document.querySelector("template.transition").innerHTML;
  
  while (container.firstChild)
    container.firstChild.remove();


    let transitionsContainer = document.querySelector('.transitions-container');

    // Создаем 10 блоков transition
    for (let i = 1; i <= exprs.length; i++) {
      // Клонируем шаблон transition
      let template = document.querySelector('.transition');
      let clone = template.content.cloneNode(true);

      // Вставляем номер блока в transition-content
      let transitionContent = clone.querySelector('#transition-content');
      console.log(exprs[i], expressionToString(exprs[i]));
      transitionContent.textContent = expressionToString(exprs[i]);

      // Добавляем блок transition в transitions-container
      transitionsContainer.appendChild(clone);
    }

  // for (let i = 0; i < exprs.length; ++i) {

  //   container.innerHTML += templateHTML;

  //   let elem = container.querySelector("#transition:last-child");
  //   console.log(elem);
    
  //   //elem[(elems.length-1)].getElementById("transition-content").InnerText = exprs[i].toString();

  //   // let newField = template.content.cloneNode(true);
  //   // console.
  //   // newField.getElementsByClassName("possible")[0].innerText = expr.toString();
  //   // container.appendChild(newField);
  // }
}

// document.addEventListener("DOMContentLoaded", () => {
//   produceTransitions();
// });
