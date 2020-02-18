'use strict'

// Fields Schema
const Field = require('../models/field')
const User = require('../models/user')

/*---------------
--- FUNCTIONS ---
----------------*/

// Get Field by Id
function getField (req, res) {
  let fieldId = req.params.fieldId

  Field.findById(fieldId, (err, product) => {
    if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`})
    if (!product) return res.status(404).send({message: `El campo no existe`})

    res.status(200).send({ product })
  })
}

// Get fields by Owner email
function getFieldsByOwner (req, res) {

  // Owner email
  let ownerEmail = req.params.ownerEmail

  // Find user Id by email  
  User.findOne({ email: ownerEmail }, (err, user) => {
    if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`})
    if (!user) return res.status(404).send({message: `No hay usuario con este email`})

    let ownerId = user._id

    // Find owner's fields
    Field.find({ 'owner': ownerId }, (err, fields) => {
      if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`})
      if (!fields) return res.status(404).send({message: `No hay campos`})

      res.status(200).send({ fields })
    })
  })
}

function getProducts (req, res) {
  Field.find({}, (err, products) => {
    if (err) return res.status(500).send({message: `Error al realizar la petición: ${err}`})
    if (!products) return res.status(404).send({message: 'No existen productos'})

    res.send(200, { products })
  })
}

function saveProduct (req, res) {
  console.log('POST /api/product')
  console.log(req.body)

  let product = new Field()
  product.name = req.body.name
  product.picture = req.body.picture
  product.price = req.body.price
  product.category = req.body.category
  product.description = req.body.description

  product.save((err, productStored) => {
    if (err) res.status(500).send({message: `Error al salvar en la base de datos: ${err} `})

    res.status(200).send({ product: productStored })
  })
}

function updateProduct (req, res) {
  let fieldId = req.params.fieldId
  let update = req.body

  Field.findByIdAndUpdate(fieldId, update, (err, productUpdated) => {
    if (err) res.status(500).send({message: `Error al actualizar el producto: ${err}`})

    res.status(200).send({ product: productUpdated })
  })
}

function deleteProduct (req, res) {
  let fieldId = req.params.fieldId

  Field.findById(fieldId, (err, product) => {
    if (err) res.status(500).send({message: `Error al borrar el producto: ${err}`})

    product.remove(err => {
      if (err) res.status(500).send({message: `Error al borrar el producto: ${err}`})
      res.status(200).send({message: 'El producto ha sido eliminado'})
    })
  })
}

module.exports = {
  getField,
  getFieldsByOwner,
  getProducts,
  saveProduct,
  updateProduct,
  deleteProduct
}
