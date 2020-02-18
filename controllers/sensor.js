'use strict'

// Sensors Schema
const Sensor = require('../models/sensor')

/*---------------
--- FUNCTIONS ---
----------------*/

// Get sensor by Id (TO DO)
function getSensor (req, res) {
  let fieldId = req.params.fieldId

  Sensor.findById(fieldId, (err, product) => {
    if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`})
    if (!product) return res.status(404).send({message: `El campo no existe`})

    res.status(200).send({ product })
  })
}

// Get sensors by field
function getSensorByField (req, res) {
  let fieldId = req.params.fieldId

  Sensor.find({ 'field': fieldId }, (err, sensors) => {
    if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`})
    if (!sensors) return res.status(404).send({message: `No hay sensores`})

    res.status(200).send({ sensors })
  })
}


module.exports = {
  getSensor,
  getSensorByField
}
