-- lua/config/dap/rust.lua

local dap = require("dap")

dap.adapters.codelldb = {
	type = "server",
	port = "${port}",
	executable = {
		-- (21) Mason 安装的 codelldb 适配器
		command = vim.fn.stdpath("data") .. "/mason/packages/codelldb/extension/adapter/codelldb",
		args = { "--port", "${port}" },
	},
}

dap.configurations.rust = {
	{
		type = "codelldb",
		request = "launch",
		name = "Launch",
		-- (22) 手动指定编译产物路径 (target/debug/<name>)
		program = function()
			return vim.fn.input("Path to executable: ", vim.fn.getcwd() .. "/target/debug/", "file")
		end,
		cwd = "${workspaceFolder}",
		stopOnEntry = false, -- (23) 不在入口停下
	},
}
