.pragma library

function clamp(value, min, max) {
    console.assert(!isNaN(value) && !isNaN(min) && !isNaN(max), "invalid input")
    console.assert(min <= max, "invalid input")
    return clampNoAssert(value, min, max)
}

function clampNoAssert(value, min, max) {
    return Math.max(min, Math.min(value, max))
}
