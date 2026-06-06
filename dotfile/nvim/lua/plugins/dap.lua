-- 文件路径: ~/.config/nvim/lua/plugins/dap.lua

return {
	-- 1. nvim-dap 核心调试引擎
	{
		"mfussenegger/nvim-dap",
		config = function() end,
		keys = {
			{
				"<leader>dB",
				function()
					require("dap").toggle_breakpoint()
				end,
				desc = "Debug: Toggle breakpoint",
			},
			{
				"<leader>dc",
				function()
					require("dap").continue()
				end,
				desc = "Debug: Continue",
			},
			{
				"<leader>di",
				function()
					require("dap").step_into()
				end,
				desc = "Debug: Step Into",
			},
			{
				"<leader>do",
				function()
					require("dap").step_over()
				end,
				desc = "Debug: Step Over",
			},
			{
				"<leader>dO",
				function()
					require("dap").step_out()
				end,
				desc = "Debug: Step Out",
			},
			{
				"<leader>dt",
				function()
					require("dap").terminate()
				end,
				desc = "Debug: Terminate",
			},
			{
				"<leader>dr",
				function()
					require("dap").repl.open()
				end,
				desc = "Debug: Open REPL",
			},
		},
	},

	-- 2. nvim-dap-ui 图形界面
	{
		"rcarriga/nvim-dap-ui",
		dependencies = {
			"mfussenegger/nvim-dap", -- 依赖核心引擎
			"nvim-neotest/nvim-nio", -- 支持UI的异步操作
		},
		config = function()
			local dap = require("dap")
			local dapui = require("dapui")
			dapui.setup()
			-- 调试会话开始时自动打开UI
			dap.listeners.after.event_initialized["dapui_config"] = function()
				dapui.open()
			end
			-- 调试会话结束时自动关闭UI
			dap.listeners.before.event_terminated["dapui_config"] = function()
				dapui.close()
			end
			dap.listeners.before.event_exited["dapui_config"] = function()
				dapui.close()
			end
			-- 映射快捷键来手动开关UI
			vim.keymap.set("n", "<leader>du", dapui.toggle, { desc = "Debug: Toggle UI" })
		end,
	},

	-- 3. 变量代码行内显示
	{
		"theHamsta/nvim-dap-virtual-text",
		dependencies = { "mfussenegger/nvim-dap" },
		config = true, -- 自动运行 setup()
	},
}
