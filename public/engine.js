



const produceTransitions = () => {

    let equation = document.getElementById("equation").value;
    
    console.log(equation);

    fetch("global.js", {
        method: "CALC",
        headers: {
          "Content-Type": "application/json",
        },
        body: equation
      })
      //.then(response => response.json())
      .then(data => {
        console.log("Ответ от сервера:", data);
      })
      .catch(error => {
        console.error("Ошибка:", error);
      });

};

