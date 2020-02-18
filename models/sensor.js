'use strict'

const mongoose = require('mongoose')
const Schema = mongoose.Schema

const SensorSchema = Schema({
  mac: String,
  field: {
    type: mongoose.Schema.Types.ObjectId,
    ref: 'Field'
  },
  latitude: String,
  altiude: String,
  area: Array
})

module.exports = mongoose.model('Sensor', SensorSchema)
