fetch("http://localhost:8080/")
  .then(response => response.json())
  .then(data => {
    console.log(data)
    document.getElementById('result').innerHTML = 
    data[data.length -1].count
  })
  .catch(error => console.error(error))
