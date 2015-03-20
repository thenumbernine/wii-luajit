function math.trunc(x)
	if x < 0 then return math.ceil(x) else return math.floor(x) end
end

function math.fpart(x)
	return x - math.floor(x)
end

function math.clamp(v,min,max)
	return math.min(math.max(v,min), max)
end

math.infinity = 1/0			-- phase this out?
math.inf = math.infinity	-- in favor of this?

math.nan = 0/0

math.twoPi = 2 * math.pi

