



const produceTransitions = () =>
{

  console.log("hello");

  let equation = document.getElementById("equation").value;
    
  console.log(equation);


  fetch("", {
    method: "CALC",
    body: equation
  })
  .then(response => response.text())
  .then(data => {
      // Создание парсера XML
      const parser = new DOMParser();
      const xmlDoc = parser.parseFromString(data, 'application/xml');

      // Извлечение данных из XML
      const transformations = xmlDoc.getElementsByTagName('transformation');
      let output = '<div class="transformations">';

      for (let i = 0; i < transformations.length; i++) {
          const description = transformations[i].getElementsByTagName('description')[0].textContent;
          const solution = transformations[i].getElementsByTagName('solution')[0].textContent;

          output += `<div class="transformation"><div class="description">${description}</div><div class="solution">${solution}</div></div>`;
      }

      output += '</div>';

      // Отображение данных на странице
      document.getElementById('xmlContent').innerHTML = output;

  })
  .catch(error => console.error('Error:', error));


}
