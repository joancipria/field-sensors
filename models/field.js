'use strict'

const mongoose = require('mongoose')
const Schema = mongoose.Schema

const FieldSchema = Schema({
  name: String,
  owner: {
    type: mongoose.Schema.Types.ObjectId,
    ref: 'User'
  },
  area: Array
})

module.exports = mongoose.model('Field', FieldSchema)
