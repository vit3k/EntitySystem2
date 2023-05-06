---@enum Events
Events = {
	Started = internal_Events.Started
}
setmetatable(Events, {__index = internal_Events})

---@enum Components
Components = {}
setmetatable(Components, {__index = internal_Components})

---@class Entity
---@field get fun(self: Entity, component: Components)
Entity = {}

---@class System
---@field name string
---@field requiredComponents Components[]
---@field processEntity fun(self: System, entity, deltaTime: number)
System = {}

---@param event Events
---@param func function
function subscribe(event, func)
	internal_subscribe(event, func)
end

function subscribeForObject(event, func, obj)
	internal_subscribeForObject(event, func, obj)
end

function createEntity(definition)
	return internal_createEntity(definition)
end

function createEntityDirect(definition)
	return internal_createEntityDirect(definition)
end

---@param system System
function registerSystem(system)
	return internal_registerSystem(system)
end

---@param event Events
---@param data any
function emit(event, data)
	internal_emit(event, data)
end

function clearWorld()
	internal_clearWorld()
end

---@enum Key
Key = {
	A = internal_Key.A,
	D = internal_Key.D
}
setmetatable(Key, {__index = internal_Key})

Input = {
	---@param actionName string
	---@param key Key
	mapAction = function(actionName, key)
		internal_Input.mapAction(actionName, key)
	end,
	---@param actionName string
	---@return boolean
	isActionPressed = function(actionName)
		return internal_Input.isActionPressed(actionName)
	end
}

---@class WindowConfiguration
---@field width number
---@field height number
---@field title string
WindowConfiguration = {}
---@class Configuration
---@field window WindowConfiguration
---@field frameLimit number
Configuration = {}

Engine = {
	---@param config Configuration
	init = function(config)
		internal_EngineInit(config)
	end
}
